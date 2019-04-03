# author: minrui

import os
import re
import ycm_core

DIR_OF_THIS_SCRIPT = os.path.abspath( os.path.dirname( __file__ ) )
DIR_OF_THIRD_PARTY = os.path.join( DIR_OF_THIS_SCRIPT, 'third_party' )
SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]

# set this to the directory which compile_commands.json
compilation_database_folder = '/home/mr/Workspace/loc/build'

compilation_database_path = os.path.join(compilation_database_folder, 'compile_commands.json')
if os.path.exists( compilation_database_path ):
  database = ycm_core.CompilationDatabase( compilation_database_folder )
else:
  database = None

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
]

def flagsFromDatabase(database_file_path):
    command_line_pattern = re.compile(r'^\s+"command":')
    include_pattern = re.compile(r'^(-I.+)\s*$')
    sys_include_pattern = re.compile(r'^\s*-isystem')
    define_pattern = re.compile(r'^(-D.+)\s*')

    database_file = open(database_file_path)
    lines = database_file.readlines()
    config_file.close()
    
    flags = set()
    tmp_flag = ""
    for line in lines:
        m = command_line_pattern.match(line)
        if m:
            segs = line.split()
            for seg in segs:
                match_inc = include_pattern.match(seg)
                match_sys_inc = sys_include_pattern.match(seg)
                match_define = define_pattern.match(seg)
                if match_inc:
                    flags.add(match_inc.group())
                elif match_sys_inc:
                    tmp_flag = seg
                elif not tmp_flag == "":
                    flag = tmp_flag+" "+seg
                    flags.add(flag)
                    tmp_flag = ""
                elif match_define:
                    flags.add(seg)
    return list(flags)
    

database_flags = []
if database:
    database_flags = flagsFromDatabase(compilation_database_path)


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

    # else file is not the database, or database not exist, use what we set previous
    return {
            'flags': database_flags + flags + extra_flag,
            'include_paths_relative_to_dir': DIR_OF_THIS_SCRIPT,
            'override_filename': filename
            }
  return {}
