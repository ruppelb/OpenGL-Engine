# OpenGL Engine
A lightweight, self-contained OpenGL-based rendering engine built in C++ that demonstrates scene management, model loading, lighting, and live UI controls.
This project is designed to be a foundation for learning and prototyping real-time rendering techniques on Windows using modern OpenGL.

## Disclaimer
### Project Status
Development has essentially stopped since I ran out of time and focused on other projects.
### Missing Dependency Binaries
To put this project on GitHub, some necessary binaries were removed because they exceeded the single-file limit of 100MB. Therefore, the project is not functional in the current state.

---

## Table of Contents

- [What it does](#what-it-does)
- [Why it’s useful](#why-its-useful)
- [Project structure](#project-structure)
- [Getting started](#getting-started)
  - [Requirements](#requirements)
  - [Build (Windows)](#build-windows)
  - [Run the engine](#run-the-engine)
- [Getting help](#getting-help)
- [Contributing](#contributing)

---

## What it does

This engine provides a minimal but complete render loop to:

- Load & render 3D meshes using Assimp (OBJ, FBX, etc.)
- Apply textures and material properties (diffuse/specular/normal maps)
- Render with basic lighting (directional + point lights)
- Navigate with a camera and adjust scene parameters in real time
- Edit objects and lights at runtime via an ImGui UI panel

The engine is designed to be extended with custom rendering effects, post-processing, or game logic.

## Why it’s useful

- ✅ **Quick start for OpenGL learning** — most dependencies are vendored, and the project builds out of the box in Visual Studio.
- ✅ **Interactive debugging** — use the ImGui panel to inspect scene objects and tweak shader parameters live.
- ✅ **Real asset pipeline** — supports loading real model formats (via Assimp) and uses standard OpenGL buffer/shader abstractions.
- ✅ **Self-contained demo** — includes shaders, materials, and sample objects in `res/` so you can run without extra setup.

## Project structure

```
OpenGL Engine/                   # Visual Studio solution and project
├─ includes/                     # Prebuilt 3rd-party libraries (GLFW, GLEW, Assimp, OpenCV)
├─ res/                          # Shaders, textures, material presets, example objects
└─ src/                          # Engine source code
   ├─ scene_objects/             # Camera, lights, and scene object types
   ├─ vendor/                    # Embedded 3rd-party source (GLM, ImGui, stb_image)
   ├─ Application.cpp            # Entry point + main loop
   ├─ SceneManager.*             # Scene management and object lifecycle
   ├─ Renderer.*                 # Rendering pipeline
   ├─ Shader.*                   # GLSL shader loading/compilation
   └─ ...
```

## Getting started

### Requirements

- Windows 10+ (tested on Windows 10/11)
- Visual Studio 2022 (or newer) with **Desktop development with C++** workload
- A GPU with OpenGL 3.0+ support

> Most 3rd‑party libraries (GLFW, GLEW, GLM, ImGui, OpenCV, Assimp, stb_image) are included in this repo under `includes/` and `src/vendor/`.

### Build (Windows)

1. Clone the repo:

   ```bash
   git clone https://github.com/<your-org>/opengl-engine.git
   cd opengl-engine
   ```

2. Open the solution:

   - `OpenGL Engine.sln`

3. Select a configuration:

   - **Configuration**: `Debug` or `Release`
   - **Platform**: `x64` (recommended) or `Win32`

4. Build the solution (`Build > Build Solution`).

> ⚠️ If you encounter missing library errors, ensure your selected platform matches the included binary libraries (the default libs are built for x64).

### Run the engine

- Run the `OpenGL Engine` project from Visual Studio (F5) or launch the executable directly:

  `OpenGL Engine\\x64\\Debug\\OpenGL Engine.exe`

- The app launches a window with an ImGui panel on the left where you can inspect/edit scene objects, lights, and camera settings.

## Getting help

- Open an issue in this repository with details of the problem.
- Explore `src/` files to understand how rendering, input, and scene management work.

## Contributing

Contributions are welcome!

1. Fork the repository
2. Create a feature branch
3. Open a pull request with a clear description of your changes
