add_library(lib_logging
	src/logging.cpp
)

target_include_directories(lib_logging PUBLIC include)
target_link_libraries(lib_logging lib_application lib_window)
