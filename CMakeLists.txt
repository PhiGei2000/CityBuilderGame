cmake_minimum_required(VERSION 3.20.0)
project(cityBuilderGame VERSION 0.1.0)

include(CTest)
enable_testing()

set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Create DEBUG definition")

    add_compile_definitions(DEBUG)
endif()

if (NOT WIN32)
    message("Create sourcelist")
    execute_process(COMMAND /bin/bash ./createSourcelist.sh)
endif()

include_directories(./include/)
include(sourcelist.cmake)

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)

add_executable(cityBuilderGame ${SOURCES})

target_include_directories(cityBuilderGame PUBLIC
    ${ENTT_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS})

if (WIN32)
    #cmake_path(SET GLEW_INCLUDE_DIRS NORMALIZE "${GLEW_DIR}/../../../include")     

    target_include_directories(cityBuilderGame PUBLIC
        ${GLM_INCLUDE_DIRS}        
        ${STB_INCLUDE_DIRS})
endif()

target_link_libraries(cityBuilderGame
    ${OPENGL_LIBRARIES}
    glfw ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

