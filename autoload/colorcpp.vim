" vim: et ts=2 sts=2 sw=2

let s:save_cpo = &cpo
set cpo&vim

let g:color_cpp_unique_window_id = 0
let g:color_cpp_matches = {}

source $VIMHOME/syntax/color_cpp_highlight.vim

function! colorcpp#enable_colorcpp()
  call s:setup_server()
  call pysupport#setup_python()
  call s:setup_auto_group()
  call s:setup_commands()
  call s:setup_keymap()
endfunction

function! s:setup_commands()
  command! ColorCppUpdateConf call s:update_ycm_extra_conf()
endfunction

function! s:update_ycm_extra_conf()
    let l:cur_file = expand('%:p')
    let l:dentries = split(cur_file, '/')[0:-2]
    let l:home_path = $HOME

    let l:ycm_extra_conf_file = ""
    let l:cur_path = "/".join(dentries, "/")
    let l:project_setting_dir = ""
    while cur_path != home_path
        let l:ycm_extra_conf_file= globpath(cur_path, ".ycm_extra_conf.py")
        if l:ycm_extra_conf_file != ""
            break
        endif

        let l:dentries = l:dentries[0:-2]
        let l:cur_path = "/".join(l:dentries, "/")
    endwhile

    if l:ycm_extra_conf_file == ""
        echo "Could not find ycm_extra_conf_file"
        return
    endif

    exec "!"
    exec "YcmCompleter ClearCompilationFlagCache"
endfunction

function! s:setup_auto_group()
  augroup color_cpp
    autocmd!
    autocmd FileType,BufEnter,WinEnter * call pysupport#parse_current_buffer()
    autocmd InsertLeave,InsertEnter,TextChanged * call pysupport#parse_current_buffer()

    autocmd WinEnter,BufEnter * call pysupport#refresh_window_highlight()
    autocmd CursorMoved,CursorMovedI * call pysupport#move_highlight()
    autocmd CursorHold,CursorHoldI   * call pysupport#move_highlight()
  augroup END
endfunction

function! s:setup_keymap()
  nnoremap <silent> <ScrollWheelUp>
        \ <ScrollWheelUp>:call pysupport#move_highlight()<CR>
  inoremap <silent> <ScrollWheelUp>
        \ <ScrollWheelUp><ESC>:call pysupport#move_highlight()<CR><INS>
  nnoremap <silent> <ScrollWheelDown>
        \ <ScrollWheelDown>:call pysupport#move_highlight()<CR>
  inoremap <silent> <ScrollWheelDown>
        \ <ScrollWheelDown><ESC>:call youcompleteme#Highlight()<CR><INS>

  nnoremap <silent> <S-ScrollWheelUp>
        \ <S-ScrollWheelUp>:call pysupport#move_highlight()<CR>
  inoremap <silent> <S-ScrollWheelUp>
        \ <S-ScrollWheelUp><ESC>:call pysupport#move_highlight()<CR><INS>
  nnoremap <silent> <S-ScrollWheelDown>
        \ <S-ScrollWheelDown>:call pysupport#move_highlight()<CR>
  inoremap <silent> <S-ScrollWheelDown>
        \ <S-ScrollWheelDown><ESC>:call pysupport#move_highlight()<CR><INS>

  nnoremap <silent> <C-ScrollWheelUp>
        \ <C-ScrollWheelUp>:call pysupport#move_highlight()<CR>
  inoremap <silent> <C-ScrollWheelUp>
        \ <C-ScrollWheelUp><ESC>:call pysupport#move_highlight()<CR><INS>
  nnoremap <silent> <C-ScrollWheelDown>
        \ <C-ScrollWheelDown>:call pysupport#move_highlight()<CR>
  inoremap <silent> <C-ScrollWheelDown>
        \ <C-ScrollWheelDown><ESC>:call pysupport#move_highlight()<CR><INS>
endfunction

function! s:setup_server() abort
  let l:job_option = {}
  let l:job_option['in_io'] = "pipe"
  let l:job_option['in_mode'] = "json"
  let l:job_option['out_io'] = "pipe"
  let l:job_option['out_mode'] = "json"
  let l:job_option['out_cb'] = "pysupport#handle_response"
  let s:server_job = job_start($VIMHOME.'/server/server', l:job_option)
  if job_status(s:server_job) == "fail"
    echom "Falid to start color-cpp server, install first"
    finish
  endif
  let g:colorcpp_job_channel = job_getchannel(s:server_job)
endfunction

function! colorcpp#clear_highlight_match(bufnr)
  try
    if has_key(g:color_cpp_matches, a:bufnr) == 1
      for id in g:color_cpp_matches[a:bufnr]
        call matchdelete(id)
      endfor
    endif
  catch
    echomsg "color_cpp caught: " . v:exception
  finally
    let g:color_cpp_matches[a:bufnr] = []
  endtry
endfunction!

function! colorcpp#add_highlight_match(bufnr, type, line, col, len)
  call add(g:color_cpp_matches[a:bufnr], matchaddpos(a:type, [[ a:line, a:col, a:len ]], -1))
  "echom a:type.",".a:line.",".a:col.",".a:len
endfunction!


let &cpo = s:save_cpo
unlet s:save_cpo
