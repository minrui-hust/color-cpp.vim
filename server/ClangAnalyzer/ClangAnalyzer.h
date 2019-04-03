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

#ifndef CLANGCOMPLETE_H_WLKDU0ZV
#define CLANGCOMPLETE_H_WLKDU0ZV

#include "UnsavedFile.h"
#include "Highlight.h"
#include "TranslationUnitStore.h"

#include <string>

using CXTranslationUnit = CXTranslationUnitImpl*;


class TranslationUnit;
struct Location;


// All filename parameters must be absolute paths.
class ClangAnalyzer{
public:
  ClangAnalyzer();
  ~ClangAnalyzer();
  ClangAnalyzer( const ClangAnalyzer& ) = delete;
  ClangAnalyzer& operator=( const ClangAnalyzer& ) = delete;

  bool UpdatingTranslationUnit( const std::string &filename );

  std::vector<Highlight> UpdateTranslationUnit(
    const std::string &translation_unit,
    const std::vector< UnsavedFile > &unsaved_files,
    const std::vector< std::string > &flags );

  void DeleteCachesForFile( const std::string &filename );

private:
  CXIndex clang_index_;

  TranslationUnitStore translation_unit_store_;
};

#endif /* end of include guard: CLANGCOMPLETE_H_WLKDU0ZV */
