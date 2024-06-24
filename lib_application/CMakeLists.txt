file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

file(GLOB_RECURSE SCENE_HEADERS scenes/*.h)
file(GLOB_RECURSE SCENE_SOURCES scenes/*.cpp)

list(APPEND HEADERS ${SCENE_HEADERS})
list(APPEND SOURCES ${SCENE_SOURCES})

add_library(lib_application ${HEADERS} ${SOURCES})

target_include_directories(lib_application PUBLIC include scenes)

target_link_libraries(lib_application PUBLIC lib_window lib_renderer lib_log)