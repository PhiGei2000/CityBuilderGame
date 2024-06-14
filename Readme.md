![Image](docs/image.jpeg)
# City Builder Game
## Overview
A simple city-building game using C++ and OpenGL.
### Features
- [x] GUI
- [x] Street placement
- [x] procedural terrain generation and terrain editing
- [ ] Traffic
- [ ] Buildings
- [ ] Savegames
- ...
---
## Getting started
### Check the requirements
Requirements:
- CMake 3.22.1 and above
- C++ 20
- OpenGL 4.5

Packages (only on Linux):
- [glm](https://github.com/g-truc/glm)
- [glfw](https://github.com/glfw/glfw)
- [glew](https://github.com/nigels-com/glew)
- [Freetype2](https://github.com/ubawurinna/freetype-windows-binaries)
- [stb image](https://github.com/nothings/stb/blob/master/stb_image.h)
- [entt](https://github.com/skypjack/entt)
- [pugixml](https://github.com/zeux/pugixml)
- [libnoise](https://github.com/PhiGei2000/libnoise)

### Clone and build
**1. Clone the repository**

    git clone --recursive https://github.com/PhiGei2000/CityBuildingGame.git

the `--recursive` option is only needed on windows to download the libraries from github.

**2. Change your current directory to the downloaded project**

    cd ./CityBuilderGame

**3. Run CMake to build the project**

    cmake --build ./build


After that keep sure you link or copy the res directory to the build directory

    cp -r ./res ./build/

or to create a symbolic link

    cd ./build && ln -s ../res res
**3. Run build script (Linux)**

    bash ./build.sh

**4. Start the game**

    ./cityBuilderGame

## Controls

| Button(s) | Description |
| --- | ---|
| WASD | Move |
| QE | Rotate camera left/right |
| RF | Rotate camera up/down |
| ESC | Main menu |
| B | Enter/Leave build mode |
| F1 | Open debug panel |
| F3 | Enter debug mode |
| F4 | Show shadow buffer |

## Issues and questions
Feel free to report issues and questions to the [issues section](https://github.com/PhiGei2000/CityBuilderGame/issues).

## Demonstration videos
Check out my [YouTube playlist](https://www.youtube.com/playlist?list=PLi1TZVQkw_fLyYXfnZGTGli4pqM7FvHVu) for demonstration of some features in the game.

## License
This code is licensed under the [GNU GLP license](LICENSE)
