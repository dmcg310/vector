add_library(lib_window
	src/window.cpp
)

target_include_directories(lib_window PUBLIC include)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(lib_window glfw OpenGL::GL)
