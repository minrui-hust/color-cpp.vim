[![996.icu](https://img.shields.io/badge/link-996.icu-red.svg)](https://996.icu)
[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg)](https://github.com/996icu/996.ICU/blob/master/LICENSE)

# color-cpp: a c/c++ semantic highlighing plugin for vim8
  Using the same config file `.ycm_extra_conf.py` with `YouCompleteMe`,
  accurate highlighting is available for c/c++ variables, keywords, functions,
  classes and so on.
  **before color-cpp enabled:**  
  ![image](https://github.com/minrui-hust/color-cpp.vim/blob/master/pre.png)  
  **after color-cpp enabled:**  
  ![image](https://github.com/minrui-hust/color-cpp.vim/blob/master/post.png)

## Requirement
- &gt;=Vim8.0(tested on Vim8.1), with `job, timer, python3(better to disable python2)` enabled
- `YouCompleteMe` should be installed with the latest version
- Linux system(Ubuntu is suggested)

## Installation
  Assume `Plug` plugin manager is used, other plugin manager should be very similar
  1. put this in you `.vimrc`, **After** `YCM`
  ``` vimrc
  Plug 'Valloric/YouCompleteMe'
  Plug 'minrui-hust/color-cpp.vim'
  ```
  and use vim command `:PlugInstall` to install it

  2. first install YCM, go to YouCompleteMe folder run:
  ```
  python3 install.py --clang-completer

  ```
  if you already have YCM installed, ignore this step

  3. go to the color-cpp folder, run:
  ``` bash
  ./install.bash
  ```
  If no error happens, it means succeed, else please refer to FAQ

## Usage
  color-cpp use the same config file as YCM, which is `.ycm_extra_conf.py`,
  so the least thing you have to do is to put a `.ycm_extra_conf.py` in you project top folder.

  The one at server/.ycm_extra_conf.py(it is hidden on linux) works perfect with c/c++, for both `YCM` and `color-cpp`.
  there is only one places you need to modify according to your project:

  ```python
  # set this to the directory which compile_commands.json exist
  compilation_database_folder = '/home/mr/Workspace/loc/build'
  # set this to the directory which compile_commands.json exist
  ```
  As you can see, that is the path of compile database file `compile_commands.json`
  created by cmake with `-DCMAKE_EXPORT_COMPILE_COMMANDS=yes`.

  Enjoy it!

## FAQ
  TODO
