let s:script_folder_path = escape(expand('<sfile>:p:h'), '\')


function! pysupport#setup_python()
  python3 << EOF
import os.path as path
import sys
import traceback
import vim

color_cpp_root_folder = path.normpath( path.join( vim.eval( 's:script_folder_path' ), '..' ) )
#print("color_cpp_root:"+color_cpp_root_folder)

ycm_root_folder = path.normpath( path.join(color_cpp_root_folder, '../YouCompleteMe') )
#print("ycm_root:"+ ycm_root_folder)

# Add dependencies to Python path
sys.path.append(path.join( color_cpp_root_folder, 'python' ))
sys.path.append(path.join( ycm_root_folder, 'third_party', 'ycmd', 'ycmd'))

from server_utils import SetUpPythonPath
SetUpPythonPath()

from utils import LoadYcmCoreDependencies
LoadYcmCoreDependencies()

from ycm import base
from ycmd import user_options_store
user_options_store.SetAll(base.GetUserOptions())

import client

cli = client.Client()
EOF
endfunction

function! pysupport#parse_current_buffer()
  python3 cli.parseCurrentBuffer(force=False)
endfunction

function! pysupport#force_parse_current_buffer()
  python3 cli.parseCurrentBuffer(force=True)
endfunction

function! pysupport#handle_response(channel, response)
  "echom "response"
  python3 cli.handleResponse(vim.eval("a:response"))
endfunction

function! pysupport#refresh_window_highlight()
    if !exists('w:unique_id')
        " this window is newly created, so assign an unique id to it
        let w:unique_id = g:color_cpp_unique_window_id
        let g:color_cpp_unique_window_id += 1
    endif

    " windows can be reused, clear the existing matches
    " this value will be set again in highlight interface
    if exists('w:color_code_name')
        call colorcpp#clear_highlight_match(w:color_code_name)
        unlet w:color_code_name
    endif

    python3 cli.refreshWindowHighlight()
endfunction

function! pysupport#move_highlight()
  exec "py3 cli.moveHighlight(".line("w0").",".line("w$")")"
endfunction

