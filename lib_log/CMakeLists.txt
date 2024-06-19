add_library(lib_log
	src/log.cpp
)

target_include_directories(lib_log PUBLIC include)
target_link_libraries(lib_log lib_application)
