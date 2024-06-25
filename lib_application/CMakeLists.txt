file(GLOB_RECURSE SOURCES src/*.cpp)

file(GLOB_RECURSE SCENE_HEADERS scenes/*.h)
file(GLOB_RECURSE SCENE_SOURCES scenes/*.cpp)

file(GLOB_RECURSE NODE_HEADERS nodes/*.h)
file(GLOB_RECURSE NODE_SOURCES nodes/*.cpp)

list(APPEND HEADERS ${SCENE_HEADERS} ${NODE_HEADERS})
list(APPEND SOURCES ${SCENE_SOURCES} ${NODE_SOURCES})

add_library(lib_application ${HEADERS} ${SOURCES})

target_include_directories(lib_application PUBLIC include scenes nodes)

target_link_libraries(lib_application PUBLIC lib_window lib_renderer lib_log)