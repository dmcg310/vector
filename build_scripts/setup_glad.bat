@echo off

set EXTERNAL_DIR=external
set GLAD_DIR=%EXTERNAL_DIR%\glad
set INCLUDE_DIR=%GLAD_DIR%\include
set GLAD_SRC=%GLAD_DIR%\src\glad.c
set GLAD_CMAKE=%GLAD_DIR%\CMakeLists.txt
set GLAD_ZIP=%EXTERNAL_DIR%\glad.zip

mkdir %GLAD_DIR%
mkdir %INCLUDE_DIR%

echo Extracting glad...
tar -xf %GLAD_ZIP% -C %GLAD_DIR%

echo Creating CMakeLists.txt for glad...
echo add_library(glad STATIC src/glad.c) > %GLAD_CMAKE%
echo target_include_directories(glad PUBLIC include) >> %GLAD_CMAKE%

echo Glad setup complete.
