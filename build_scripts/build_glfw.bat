@echo off
setlocal

set GLFW_VERSION=3.4
set GLFW_SOURCE_DIR=%cd%\external\glfw
set GLFW_BUILD_DIR=%cd%\external\glfw_build
set GLFW_INSTALL_DIR=%cd%\external\glfw_install

if not exist %GLFW_SOURCE_DIR% mkdir %GLFW_SOURCE_DIR%
if not exist %GLFW_BUILD_DIR% mkdir %GLFW_BUILD_DIR%
if not exist %GLFW_INSTALL_DIR% mkdir %GLFW_INSTALL_DIR%

cd %GLFW_SOURCE_DIR%
if not exist glfw-%GLFW_VERSION%.zip (
    curl -LO https://github.com/glfw/glfw/releases/download/%GLFW_VERSION%/glfw-%GLFW_VERSION%.zip
    tar -xf glfw-%GLFW_VERSION%.zip --strip-components=1
)

cd %GLFW_BUILD_DIR%
cmake -S %GLFW_SOURCE_DIR% -B %GLFW_BUILD_DIR% -DCMAKE_INSTALL_PREFIX=%GLFW_INSTALL_DIR% -DBUILD_SHARED_LIBS=ON
cmake --build %GLFW_BUILD_DIR% --config Release --target install

endlocal