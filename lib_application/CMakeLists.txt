add_library(lib_application
	src/application.cpp
)

target_include_directories(lib_application PUBLIC include)
target_link_libraries(lib_application lib_window)