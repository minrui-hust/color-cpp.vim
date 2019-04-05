[![996.icu](https://img.shields.io/badge/link-996.icu-red.svg)](https://996.icu)
[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg)](https://github.com/996icu/996.ICU/blob/master/LICENSE)

# color-cpp: a c/c++ semantic highlighing plugin for vim8
  using the same config file `.ycm_extra_conf.py` with `YouCompleteMe`,
  accurate highlighting is available for c/c++ variables, keywords, functions,
  classes and so on.  
  **before color-cpp enabled:**  
  ![image](https://github.com/minrui-hust/color-cpp.vim/blob/master/pre.png)  
  **after color-cpp enabled:**  
  ![image](https://github.com/minrui-hust/color-cpp.vim/blob/master/post.png)

## Requirement
- &gt;=Vim8.0(tested on Vim8.1), with `job, timer, python3` enabled
- `YouCompleteMe` should be installed and be the latest version
- Linux system(Ubuntu is suggested)

## Installation
  assume `Plug` plugin manager is used, other plugin manager should be very similar
  1. put this in you `.vimrc`, **After** `YCM`
  ``` vimrc
  Plug 'Valloric/YouCompleteMe'
  Plug 'minrui-hust/color-cpp.vim'
  ```
  and use vim command `:PlugInstall` to install it

  2. go to the color-cpp folder, run:
  ``` bash
  ./install.bash
  ```
  if no error happens, it means succeed, else please refer to FAQ

## Usage
  color-cpp use the same config file as YCM, which usually is `.ycm_extra_conf.py`,
  so the least thing you have to do is to put a `.ycm_extra_conf.py` in you project top folder,
  if you do not have one, the one in tools/ works fine with c/c++, for both `YCM` and `color-cpp`.
  there is three places you can modify according to your project:
  ```python
  # set this to the directory which compile_commands.json exist
  compilation_database_folder = '/home/mr/Workspace/loc/build'
  # set this to the directory which compile_commands.json exist
  
  
  #extra flags for both source and header files
  extra_flag = [
  '-I/usr/include/c++/5',
  '-I/usr/include',
  ]
  #extra flags for both source and header files
  
  
  # flags for header files, and source file not in database
  flags = [
  '-x',
  'c++',
  '-std=c++14',
  ]
  # flags for header files, and source file not in database
  ```
  as you can see, the most important one is the `compile_commands.json` file
  created by cmake with `-DCMAKE_EXPORT_COMPILE_COMMANDS=yes`.

## FAQ
