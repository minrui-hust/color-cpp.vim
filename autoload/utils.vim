let s:plugin_home = expand('<sfile>:p:h:h')

function! utils#install()
  execute 'lcd ' . s:plugin_home
  execute '!' . './install.bash'
endfunction
