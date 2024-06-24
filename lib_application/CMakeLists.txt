file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

file(GLOB_RECURSE SOURCES scenes/*.cpp)
file(GLOB_RECURSE SOURCES scenes/*.h)

add_library(lib_application ${HEADERS} ${SOURCES})

# Include directories
target_include_directories(lib_application PUBLIC include)

# Link libraries
target_link_libraries(lib_application PUBLIC lib_window lib_renderer lib_log)
