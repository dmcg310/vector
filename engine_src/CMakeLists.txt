file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(engine_main ${SOURCES})

# Link libraries
target_link_libraries(engine_main lib_application lib_window lib_renderer lib_log)