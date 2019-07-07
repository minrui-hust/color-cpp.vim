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

#include "ClangAnalyzer.h"
#include "ClangUtils.h"
#include "TranslationUnit.h"

#include <clang-c/Index.h>
#include <memory>
#include <string>
#include <iostream>
#include <time.h>
#include <stdio.h>


using std::shared_ptr;

ClangAnalyzer::ClangAnalyzer()
  : clang_index_( clang_createIndex( 0, 0 ) ),
    translation_unit_store_( clang_index_ ) {
  // The libclang docs don't say what is the default value for crash recovery.
  // I'm pretty sure it's turned on by default, but I'm not going to take any
  // chances.
  clang_toggleCrashRecovery( true );
}


ClangAnalyzer::~ClangAnalyzer() {
  // We need to destroy all TUs before calling clang_disposeIndex because
  // the translation units need to be destroyed before the index is destroyed.
  // Technically, a thread could still be holding onto a shared_ptr<TU> object
  // when we destroy the clang index, but since we're shutting down, we don't
  // really care.
  // In practice, this situation shouldn't happen because the server threads are
  // Python deamon threads and will all be killed before the main thread exits.
  translation_unit_store_.RemoveAll();
  clang_disposeIndex( clang_index_ );
}


bool ClangAnalyzer::UpdatingTranslationUnit( const std::string &filename ) {
  shared_ptr< TranslationUnit > unit = translation_unit_store_.Get( filename );

  if ( !unit ) {
    return false;
  }

  // Thankfully, an invalid, sentinel TU always returns true for
  // IsCurrentlyUpdating, so no caller will try to rely on the TU object, even
  // if unit is currently pointing to a sentinel.
  return unit->IsCurrentlyUpdating();
}


std::vector<Highlight> ClangAnalyzer::UpdateTranslationUnit(
  const std::string &translation_unit,
  const std::vector< UnsavedFile > &unsaved_files,
  const std::vector< std::string > &flags ) {
  bool translation_unit_created;

  shared_ptr< TranslationUnit > unit;
  try {
    unit = translation_unit_store_.GetOrCreate(
        translation_unit,
        unsaved_files,
        flags,
        translation_unit_created );
  } catch(const ClangParseError &){
    std::cerr<<__FILE__<<":"<<__FILE__<<" Get or create TU failed"<<std::endl;
    return std::vector<Highlight>();
  }

  try {
    return unit->Reparse( unsaved_files );
  } catch ( const ClangParseError & ) {
    // If unit->Reparse fails, then the underlying TranslationUnit object is not
    // valid anymore and needs to be destroyed and removed from the filename ->
    // TU map.
    translation_unit_store_.Remove( translation_unit );
    std::cerr<<__FILE__<<":"<<__FILE__<<" Parse TU failed"<<std::endl;
    return std::vector<Highlight>();
  }
}
