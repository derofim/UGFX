UGFX - Cross-platform rendering library
=============

[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)

Welcome to the UGFX source code!  

UGFX is a free lightweight, cross-platform 2D and 3D rendering library implemented in C++ and released under the MPL 2.0 license. It is totally free and cross-platform.  

## Features

 * Free, MPL 2.0 license (OpenSource).
 * Cross-platform (including html5 target).
 * C++14 and above.
 * 2D and 3D.

 Supported platforms:

 * Android (14+, ARM, x86, MIPS)
 * asm.js/Emscripten (1.25.0)
 * Linux
 * Windows (XP, Vista, 7, 8, 10)

Supported compilers:

 * Clang
 * GCC
 * vs2015
 
Languages:

 * C/C++

## How to build

1. Clone the repository
```
git clone https://github.com/derofim/UGFX.git .
```
2. Make sure that submodules are checked out and up-to-date:
```
git submodule update --init --recursive
```

## Tools and third-party libraries

* SDL2 is a cross-platform multimedia library designed to provide fast hardware access.

## Credits

Development, contributions and bugfixes by:
- Denis Trofimov (derofim  @  yandex.ru)

## License

Licensed under the MPL 2.0 license, see LICENSE.txt for details.  

Virtually any software may use UGFX:  
* Closed-source software may use UGFX without having to disclose its own source code. 
* Many proprietary and closed-source software projects (as well as many BSD-licensed projects) may use UGFX.  

Please note that the textures, 3D models and levels are copyright by their authors and not covered by the UGFX license.

## Contributing

As an open source project, UGFX benefits greatly from both the volunteer work of helpful developers and good bug reports made by users.  

[Workflow of contributing code](https://guides.github.com/introduction/flow/):  
1. Work on your own branch. For each feature or bug fix, it should create a branch before submitting a patch or pull request.   
2. Branch should follow a naming rule. It should be named as feature/YOUR_FEATURE_NAME or bugfix/YOUR_BUG_NAME.  
3. Always base on latest development branch. Use "git pull --rebase" to rewind your changes on top of development branch.  
4. Commit.  

## Coding Style

C++ code style written in markdown: https://gist.github.com/derofim/df604f2bf65a506223464e3ffd96a78a