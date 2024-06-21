@echo off
setlocal

REM --- Save the initial directory ---
set INITIAL_DIR=%cd%

REM --- Setup GLFW ---
set GLFW_VERSION=3.4
set EXTERNAL_DIR=%cd%\external
set GLFW_SOURCE_DIR=%EXTERNAL_DIR%\glfw
set GLFW_BUILD_DIR=%EXTERNAL_DIR%\glfw_build
set GLFW_INSTALL_DIR=%EXTERNAL_DIR%\glfw_install

if not exist %GLFW_SOURCE_DIR% mkdir %GLFW_SOURCE_DIR%
if not exist %GLFW_BUILD_DIR% mkdir %GLFW_BUILD_DIR%
if not exist %GLFW_INSTALL_DIR% mkdir %GLFW_INSTALL_DIR%

cd %GLFW_SOURCE_DIR%
if not exist glfw-%GLFW_VERSION%.zip (
    echo Downloading GLFW...
    curl -LO https://github.com/glfw/glfw/releases/download/%GLFW_VERSION%/glfw-%GLFW_VERSION%.zip
    tar -xf glfw-%GLFW_VERSION%.zip --strip-components=1
)

cd %GLFW_BUILD_DIR%
echo Building and installing GLFW...
cmake -S %GLFW_SOURCE_DIR% -B %GLFW_BUILD_DIR% -DCMAKE_INSTALL_PREFIX=%GLFW_INSTALL_DIR% -DBUILD_SHARED_LIBS=ON
cmake --build %GLFW_BUILD_DIR% --config Release --target install

REM Return to the initial directory
cd %INITIAL_DIR%

REM --- Setup glad ---
set GLAD_DIR=%EXTERNAL_DIR%\glad
set GLAD_INCLUDE_DIR=%GLAD_DIR%\include
set GLAD_SRC_DIR=%GLAD_DIR%\src
set GLAD_CMAKE=%GLAD_DIR%\CMakeLists.txt
set GLAD_ZIP=%EXTERNAL_DIR%\glad.zip

if not exist %GLAD_DIR% mkdir %GLAD_DIR%
if not exist %GLAD_INCLUDE_DIR% mkdir %GLAD_INCLUDE_DIR%
if not exist %GLAD_SRC_DIR% mkdir %GLAD_SRC_DIR%

echo Extracting glad...
tar -xf %GLAD_ZIP% -C %GLAD_DIR%

echo Creating CMakeLists.txt for glad...
echo add_library\(glad STATIC src/glad.c\) > %GLAD_CMAKE%
echo target_include_directories\(glad PUBLIC include\) >> %GLAD_CMAKE%

echo Glad setup complete.

REM --- Add ImGui as submodule and setup CMakeLists.txt ---
cd %INITIAL_DIR%

set IMGUI_DIR=lib_imgui
set IMGUI_REPO=https://github.com/ocornut/imgui.git
set OTHER_DIR=%cd%\other
set TEMP_CMAKE_FILE=%OTHER_DIR%\CMakeLists.txt

REM Ensure the temporary CMakeLists.txt exists
if not exist %TEMP_CMAKE_FILE% (
    echo Error: Temporary CMakeLists.txt file does not exist.
    exit /b 1
)

if exist %IMGUI_DIR% (
    echo Updating ImGui submodule...
    git submodule update --init --recursive
) else (
    echo Adding ImGui as a submodule...
    git submodule add %IMGUI_REPO% %IMGUI_DIR%
    git submodule update --init --recursive
)

REM Move the temporary CMakeLists.txt to lib_imgui directory
copy %TEMP_CMAKE_FILE% %IMGUI_DIR%\CMakeLists.txt

if %ERRORLEVEL% neq 0 (
    echo Error: Failed to move CMakeLists.txt for ImGui.
    exit /b 1
)

echo Imgui Setup Complete

endlocal