@echo off
setlocal

REM --- Save the initial directory ---
set INITIAL_DIR=%cd%

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

REM Change directory to lib_imgui and checkout the docking branch
cd %IMGUI_DIR%
echo Checking out docking branch...
git checkout docking

REM Move the temporary CMakeLists.txt to lib_imgui directory
cd %INITIAL_DIR%
copy %TEMP_CMAKE_FILE% %IMGUI_DIR%\CMakeLists.txt

if %ERRORLEVEL% neq 0 (
    echo Error: Failed to move CMakeLists.txt for ImGui.
    exit /b 1
)

echo Imgui Setup Complete

endlocal
