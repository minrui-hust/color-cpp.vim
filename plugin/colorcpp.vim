" vim: et ts=2 sts=2 sw=2

let s:save_cpo = &cpo
set cpo&vim

function! s:restore_cpo()
    let &cpo = s:save_cpo
    unlet s:save_cpo
endfunction

function! s:has_features()
  return ((has('python') || has('python3'))
        \ && has('job') && has('timers')) || has('nvim')
endfunction

if exists('g:color_cpp_loaded')
    call s:restore_cpo()
    finish
elseif !exists('g:loaded_youcompleteme')
    call s:restore_cpo()
    echohl WarningMsg
    echomsg 'color-cpp requires YouCompleteMe loaded first'
    echohl None
    finish
elseif !s:has_features()
    echohl WarningMsg
    echomsg 'color-cpp requires vim compiled with python or python3 and has features `job`, `timers` and `lambda`'
    echohl None
    call s:restore_cpo()
    finish
endif

let g:color_cpp_loaded = 1
let $VIMHOME = expand('<sfile>:p:h:h')

augroup colorcpp_enable
  autocmd!
  autocmd VimEnter * call colorcpp#enable_colorcpp()
augroup END

call s:restore_cpo()
