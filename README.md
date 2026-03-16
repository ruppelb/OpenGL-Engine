# OpenGL Engine
A modular C++ OpenGL rendering engine built for experimenting with modern graphics programming, engine architecture, and real-time rendering techniques.
The project provides a foundation for building interactive 3D applications or games, focusing on a clean structure, extensibility, and practical learning of graphics programming concepts.

### Disclaimer: Project Status
Development has essentially stopped since I ran out of time and focused on other projects.

## Goals of the Project
This engine aims to:
- Explore modern graphics programming
- Provide a playground for rendering techniques
- Experiment with engine architecture patterns

## Features
- Modern OpenGL rendering pipeline
- Modular engine architecture
- Asset management and resource loading
- CMake-based build system
- External dependencies handled through extern/
- Example assets and engine components included
- UI layer integration
- Cross-platform oriented project structure

## Project Structure
    OpenGL-Engine
    ├── engine/            # Core engine implementation
    ├── assets/            # Models, textures, and other resources
    ├── asset-converter/   # Tools for processing assets
    ├── ui/                # User interface layer
    ├── extern/            # External dependencies / third-party libraries
    ├── cmake/             # CMake configuration modules
    ├── CMakeLists.txt     # Root CMake build configuration

## Requirements
Typical requirements for building the engine:
- C++17 or newer
- CMake
- OpenGL-compatible GPU
- A modern compiler:
    - GCC / Clang
    - MSVC

Platform support depends on installed graphics drivers and dependencies.

## Building the Project
1. Clone the repository
    ```
    git clone https://github.com/ruppelb/OpenGL-Engine.git
    cd OpenGL-Engine
    ```
2. Generate build files
    ```
    cmake -B build
    ```
3. Compile
    ```
    cmake --build build
    ```
4. Run\
The compiled executable will be located in the `build` directory.

