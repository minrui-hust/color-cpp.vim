
link_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../YouCompleteMe/third_party/ycmd
  )

add_library(clang_analyzer
  ClangAnalyzer.cpp
  ClangHelpers.cpp
  ClangUtils.cpp
  Range.cpp
  TranslationUnit.cpp
  TranslationUnitStore.cpp
  )
