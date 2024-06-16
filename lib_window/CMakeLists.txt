add_library(lib_window
	src/window.cpp
)

target_include_directories(lib_window PUBLIC include)
find_package(glfw3 REQUIRED)
target_link_libraries(lib_window glfw)
