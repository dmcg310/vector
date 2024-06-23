option(ENABLE_OPENGL "Enable OpenGL support" ON)

file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(lib_renderer ${HEADERS} ${SOURCES})

if (APPLE OR UNIX)
	find_package(glfw3 REQUIRED)
endif()

# GLM
include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

target_include_directories(lib_renderer PUBLIC include)

if(ENABLE_OPENGL)
    file(GLOB_RECURSE OPENGL_HEADERS include/concrete/opengl/*.h)
    file(GLOB_RECURSE OPENGL_SOURCES src/concrete/opengl/*.cpp)
    target_sources(lib_renderer PRIVATE ${OPENGL_HEADERS} ${OPENGL_SOURCES})
    target_compile_definitions(lib_renderer PRIVATE ENABLE_OPENGL)

    find_package(OpenGL REQUIRED)
    target_link_libraries(lib_renderer PRIVATE OpenGL::GL glad glm::glm glfw)
endif()
