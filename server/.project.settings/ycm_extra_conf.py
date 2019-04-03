# author: minrui

from distutils.sysconfig import get_python_inc
import platform
import os
import subprocess
import ycm_core

DIR_OF_THIS_SCRIPT = os.path.abspath( os.path.dirname( __file__ ) )
DIR_OF_THIRD_PARTY = os.path.join( DIR_OF_THIS_SCRIPT, 'third_party' )
SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]

# set this to the directory which compile_commands.json
compilation_database_folder = '/home/mr/.vim/bundle/color-cpp/server/build'

if os.path.exists( os.path.join(compilation_database_folder, 'compile_commands.json') ):
  database = ycm_core.CompilationDatabase( compilation_database_folder )
else:
  database = None

# These are the compilation flags that will be used in case there's no
# compilation database set (by default, one is not set).
# CHANGE THIS LIST OF FLAGS. YES, THIS IS THE DROID YOU HAVE BEEN LOOKING FOR.

#extra flags for both source and header files
extra_flag = [
'-I/usr/include/c++/5',
'-I/usr/include',
]

# flags for header files
flags = [
'-x',
'c++',
'-std=c++14',
# PERL_GEN_BEGIN
'-I/home/mr/.vim/bundle/color-cpp/server/../../YouCompleteMe/third_party/ycmd/clang_includes',
'-I/home/mr/.vim/bundle/color-cpp/server/../../YouCompleteMe/third_party/ycmd/cpp/llvm/include',
'-I/usr/local/include',
# PERL_GEN_END
]

# this is the only function that YCM needed
def Settings( **kwargs ):
  if kwargs[ 'language' ] == 'cfamily':
    filename = kwargs['filename']
    # if this file is in the database, use flags in database
    if database:
      compilation_info = database.GetCompilationInfoForFile( filename )
      if compilation_info.compiler_flags_:
        return {
                'flags': list(compilation_info.compiler_flags_) + extra_flag,
                'include_paths_relative_to_dir': compilation_info.compiler_working_dir_,
                'override_filename': filename
                }
    # else file is not the database, use what we set previous
    return {
            'flags': flags+extra_flag,
            'include_paths_relative_to_dir': DIR_OF_THIS_SCRIPT,
            'override_filename': filename
            }
  return {}
