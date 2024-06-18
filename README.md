# engine

Build GLFW

Windows:
`.\build_scripts\build_glfw.bat`

Building the engine

Windows:
`mkdir build`
`cd build`
`cmake -s .. -B .`
Debug - `cmake --build . --config Debug` Exe found in engine\build\engine_src\Debug\engine_main.exe
Release - `cmake --build . --config Release` Exe found in engine\build\engine_src\Release\engine_main.exe
