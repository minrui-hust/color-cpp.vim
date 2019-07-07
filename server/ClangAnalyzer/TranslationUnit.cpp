// Copyright (C) 2011-2018 ycmd contributors
//
// This file is part of ycmd.
//
// ycmd is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ycmd is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ycmd.  If not, see <http://www.gnu.org/licenses/>.

#include "TranslationUnit.h"
#include "ClangHelpers.h"
#include "ClangUtils.h"
#include "TokenKindMap.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>
#include <time.h>
#include <iostream>

#include "glog/logging.h"

using std::mutex;
using std::remove_pointer;
using std::shared_ptr;
using std::try_to_lock_t;
using std::unique_lock;

namespace {

unsigned EditingOptions() {
  // See cpp/llvm/include/clang-c/Index.h file for detail on these options.
  return CXTranslationUnit_DetailedPreprocessingRecord |
         CXTranslationUnit_Incomplete |
         CXTranslationUnit_IncludeBriefCommentsInCodeCompletion |
         CXTranslationUnit_CreatePreambleOnFirstParse |
         CXTranslationUnit_KeepGoing |
         clang_defaultEditingTranslationUnitOptions();
}

unsigned ReparseOptions(CXTranslationUnit translationUnit) {
  return clang_defaultReparseOptions(translationUnit);
}

void EnsureCompilerNamePresent(std::vector<const char *> &flags) {
  bool no_compiler_name_set = !flags.empty() && flags.front()[0] == '-';

  if (flags.empty() || no_compiler_name_set) {
    flags.insert(flags.begin(), "clang");
  }
}

} // unnamed namespace

using CodeCompleteResultsWrap =
    shared_ptr<remove_pointer<CXCodeCompleteResults>::type>;

TranslationUnit::TranslationUnit() : clang_translation_unit_(nullptr) {}

TranslationUnit::TranslationUnit(const std::string &filename,
                                 const std::vector<UnsavedFile> &unsaved_files,
                                 const std::vector<std::string> &flags,
                                 CXIndex clang_index)
    : filename_(filename), clang_translation_unit_(nullptr) {
  std::vector<const char *> pointer_flags;
  pointer_flags.reserve(flags.size());

  for (const std::string &flag : flags) {
    pointer_flags.push_back(flag.c_str());
  }

  EnsureCompilerNamePresent(pointer_flags);

  std::vector<CXUnsavedFile> cxunsaved_files = ToCXUnsavedFiles(unsaved_files);
  const CXUnsavedFile *unsaved =
      cxunsaved_files.empty() ? nullptr : &cxunsaved_files[0];

  // Actually parse the translation unit.
  CXErrorCode failure = clang_parseTranslationUnit2FullArgv(
      clang_index, filename.c_str(), &pointer_flags[0], pointer_flags.size(),
      const_cast<CXUnsavedFile *>(unsaved), cxunsaved_files.size(),
      EditingOptions(), &clang_translation_unit_);
  if (failure != CXError_Success) {
    std::cerr<<__FILE__<<":"<<__LINE__<<" Clang Parse Error"<<std::endl;
    throw ClangParseError(failure);
  }
}

TranslationUnit::~TranslationUnit() { Destroy(); }

void TranslationUnit::Destroy() {
  unique_lock<mutex> lock(clang_access_mutex_);

  if (clang_translation_unit_) {
    clang_disposeTranslationUnit(clang_translation_unit_);
    clang_translation_unit_ = nullptr;
  }
}

bool TranslationUnit::IsCurrentlyUpdating() const {
  // We return true when the TU is invalid; an invalid TU also acts a sentinel,
  // preventing other threads from trying to use it.
  if (!clang_translation_unit_) {
    return true;
  }

  unique_lock<mutex> lock(clang_access_mutex_, try_to_lock_t());
  return !lock.owns_lock();
}

std::vector<Highlight>
TranslationUnit::Reparse(const std::vector<UnsavedFile> &unsaved_files) {
  std::vector<CXUnsavedFile> cxunsaved_files = ToCXUnsavedFiles(unsaved_files);

  Reparse(cxunsaved_files);

  unique_lock<mutex> lock(highlights_mutex_);
  return latest_highlights_;
}

// Argument taken as non-const ref because we need to be able to pass a
// non-const pointer to clang. This function (and clang too) will not modify the
// param though.
void TranslationUnit::Reparse(std::vector<CXUnsavedFile> &unsaved_files) {
  unsigned options =
      (clang_translation_unit_ ? ReparseOptions(clang_translation_unit_)
                               : static_cast<unsigned>(CXReparse_None));

  Reparse(unsaved_files, options);
}

// Argument taken as non-const ref because we need to be able to pass a
// non-const pointer to clang. This function (and clang too) will not modify the
// param though.
void TranslationUnit::Reparse(std::vector<CXUnsavedFile> &unsaved_files,
                              size_t parse_options) {
  CXErrorCode failure;
  {
    unique_lock<mutex> lock(clang_access_mutex_);

    if (!clang_translation_unit_) {
      return;
    }

    CXUnsavedFile *unsaved =
        unsaved_files.empty() ? nullptr : &unsaved_files[0];

    // This function should technically return a CXErrorCode enum but return an
    // int instead.
    failure = static_cast<CXErrorCode>(clang_reparseTranslationUnit(
        clang_translation_unit_, unsaved_files.size(), unsaved, parse_options));
  }

  if (failure != CXError_Success) {
    std::cerr<<__FILE__<<":"<<__LINE__<<" Clang Parse Error"<<std::endl;
    Destroy();
    throw ClangParseError(failure);
  }

  UpdateLatestHighlights();
}

void TranslationUnit::UpdateLatestHighlights() {
  unique_lock<mutex> lock1(clang_access_mutex_);
  unique_lock<mutex> lock2(highlights_mutex_);

  latest_highlights_.clear();
  unsigned int num_tokens;
  CXToken *tokens;
  CXSourceRange range = SourceRange();
  clang_tokenize(clang_translation_unit_, range, &tokens, &num_tokens);

  std::vector<CXCursor> cursors(num_tokens);
  clang_annotateTokens(clang_translation_unit_, tokens, num_tokens,
                       cursors.data());
  for (size_t i = 0; i < num_tokens; ++i) {
    CXToken token = tokens[i];
    CXTokenKind kind{clang_getTokenKind(token)};
    auto loc{clang_getTokenLocation(clang_translation_unit_, token)};

    CXFile file;
    unsigned line, column, offset;
    clang_getFileLocation(loc, &file, &line, &column, &offset);

    auto cursor_kind(cursors[i].kind);
    auto cursor_type(clang_getCursorType(cursors[i]).kind);

    CXString spell{clang_getTokenSpelling(clang_translation_unit_, token)};
    std::string text{clang_getCString(spell)};
    clang_disposeString(spell);

    auto mapped(map_token_kind(kind, cursor_kind, cursor_type));
    if (mapped.size()) {
      Highlight highlight = BuildHighlight(text, mapped, line, column);
      latest_highlights_.push_back(highlight);
    }
  }
  clang_disposeTokens(clang_translation_unit_, tokens, num_tokens);
}

CXSourceRange TranslationUnit::SourceRange() {
  // ge the whole range of the file
  size_t size;
  CXFile const file{clang_getFile(clang_translation_unit_, filename_.c_str())};
  clang_getFileContents(clang_translation_unit_, file, &size);

  CXSourceLocation const top(
      clang_getLocationForOffset(clang_translation_unit_, file, 0));
  CXSourceLocation const bottom(
      clang_getLocationForOffset(clang_translation_unit_, file, size));

  if (clang_equalLocations(top, clang_getNullLocation()) ||
      clang_equalLocations(bottom, clang_getNullLocation())) {
    throw std::runtime_error{"cannot retrieve location"};
  }

  CXSourceRange const range(clang_getRange(top, bottom));
  if (clang_Range_isNull(range)) {
    throw std::runtime_error{"cannot retrieve range"};
  }

  return range;
}
