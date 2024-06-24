add_library(lib_window
	src/window.cpp
	src/event.cpp
	src/imgui_impl_glfw.cpp
	src/imgui_impl_opengl3.cpp
	src/imgui_manager.cpp
	src/imgui_manager.cpp)

target_include_directories(lib_window PUBLIC include)

if (APPLE OR UNIX)
	find_package(glfw3 REQUIRED)
endif()

find_package(OpenGL REQUIRED)
target_link_libraries(lib_window glfw OpenGL::GL glad lib_imgui imgui)