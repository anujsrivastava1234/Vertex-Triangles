# OpenGL Traingle Example
This repository contains a basic example of creating a triangle using OpenGL, vertex shaders, and fragment shaders. This project is intended to help beginners understand the fundamentals of OpenGL rendering on a Linux system.

## Installation

### Prerequisities

Before you begin, ensure you have the following software installed:

- C++ Compiler: GCC or Clang
- CMake: Version 3.10 or higher
- OpenGL: Version 4.1 or higher
- SDL2: For handling window creation and input
- GLAD: For loading OpenGL functions

  ## Installing SDL2
  To install SDL2 on a Debian-based system, run:
  `````
  sudo apt-get update
  sudo apt-get install libsdl2-dev

  `````

  ## Installing GLAD
  GLAD can be installed by downloading the GLAD loader:

- Go to the GLAD website.
  - Select the appropriate settings:
  - API: OpenGL
  - Profile: Core
  - Version: 4.1
- Generate the loader and download the ZIP file.
- Extract the contents to your project directory.

## Test
``````
 g++ -std=c++17 main.cpp ./src/glad.c  -I./include/ -o prog -lSDL2 -ldl
``````

## Output
![Screenshot from 2024-07-09 16-02-24](https://github.com/anujsrivastava1234/Vertex-Triangles/assets/77829531/e68e104a-c4ec-4dbb-bb35-791557ab2e7b)

