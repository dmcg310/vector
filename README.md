# vector

## Setup

Windows:
`.\build_scripts\imgui.bat` from cwd: vector/

Unix:
`./build_scripts/imgui.sh` from cwd: vector/

## Building the engine

Windows:

Open in Visual Studio as a CMake project. Generate CMake files, then build as either Debug or Release, and run.


Linux:
`mkdir build`
`cd build`
`cmake -DCMAKE_BUILD_TYPE=Debug ..` or `cmake -DCMAKE_BUILD_TYPE=Release ..`
`make`

Binary found in vector/build/engine_src/engine_main
