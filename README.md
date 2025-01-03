# Phase Jump Pro
Game Design Patterns common to all games.

## Project Goals
The goal of this project is to establish a library of platform-neutral, language-neutral and engine-neutral design patterns for games. Originally written for Unity, the library has been rethought and rewritten in C++.

The Unity code still works but it is using older design patterns and needs to be updated.

## Mac Setup Instructions
- Clone the repository and submodules

### Install SDL3.framework
- Open the SDL Xcode project and archive the framework.
- Show the archive in the Finder, right click and show package contents. Copy `SDL3.framework` to `Library/Frameworks`

### Install SDL3_image.framework
- Open the SDL_image Xcode project and archive the framework
- Show the archive in the Finder, right click and show package contents. Copy `SDL3_image.framework` to `Library/Frameworks`

### Run Kaiju app
- Open the Kaiju.workspace file and run the `Kaiju` or `Kaiju-Dev` target

## Windows Setup Instructions
- TODO: create a CMake/Visual Studio project + setup instructions for Windows (on the todo list)
