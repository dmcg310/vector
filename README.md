# vector

Vector is a small, work in progress, 2D/3D game engine written in C++. Currently using OpenGL for rendering, but plan to integrate other graphics APIs in the future.

## Features

- 2D/3D rendering
- Scene graph
- Shaders
- Textures
- Camera system
- Phong lighting
- Input handling
- ImGui integration

## Dependencies

To build and run Vector, you will need the following dependencies:

- **Cmake** (Version 3.25 or higher)
- **vcpkg** (For installing dependencies)
- **C++17** compatible compiler
- **OpenGL** (Version 3.3 or higher)

## Installation Instructions

### 1. Install vcpkg

Clone the vcpkg repository and run the bootstrap script:

```sh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh # Unix
.\bootstrap-vcpkg.bat # Windows
```

### 2. Install Required Packages

Use vcpkg to install the required packages:

```sh
./vcpkg install glfw3 glm glad imgui[opengl3-binding] imgui[docking-experimental] imgui[glfw-binding]
```

### 3. Clone the Repository

```sh
git clone https://github.com/dmcg310/vector.git
```

### 4. Build the Project

#### MacOS/Linux

1. Create a build directory and navigate to it:

```sh
cd vector
mkdir build
cd build
```

2. Run CMake with the vckpg toolchain file and specify the build type:

Note: Replace `[...]` with the path to the vcpkg directory, e.g. `/home/user/vcpkg`.

Debug:

```sh
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=[...]/scripts/buildsystems/vcpkg.cmake ..
```

Release:

```sh
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=[...]/scripts/buildsystems/vcpkg.cmake ..
```

3. Build the project:

```sh
make
```

#### Windows

1. Create a build directory and navigate to it:

```sh
cd vector
mkdir build
cd build
```

2. Run CMake with the vckpg toolchain file and specify the build type:

Note: Replace `[...]` with the path to the vcpkg directory, e.g. `C:/vcpkg`.

Debug:

```sh
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=[...]/scripts/buildsystems/vcpkg.cmake ..
```

Release:

```sh
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=[...]/scripts/buildsystems/vcpkg.cmake ..
```

3. Build the project:

```sh
cmake --build .
```

### 5. Run the Project

Note: From within the build directory, e.g. `vector/build`.

#### MacOS/Linux

```sh
./application/application
```

#### Windows

Debug:

```sh
.\application\Debug\application.exe
```

Release:

```sh
.\application\Release\application.exe
```

## Platform-Specific Notes

### Windows

Ensure that you have the Visual Studio installed.

### MacOS

Ensure that you have the Xcode command line tools installed.

### Linux

Ensure that you have the necessary development packages installed.
You may need to install OpenGL development libraries, e.g. `libgl1-mesa-dev`.

## License

This project is licensed under the MIT License.