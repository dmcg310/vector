file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(lib_log ${HEADERS} ${SOURCES})

# Include directories
target_include_directories(lib_log PUBLIC include)
