cmake_minimum_required(VERSION 3.22.1)
set(PROJECT_NAME cityBuilderGame)

project(${PROJECT_NAME} VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Create DEBUG definition")

    add_compile_definitions(DEBUG)
endif()

include_directories(./include/)

if(WIN32)
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};C:/lib)
endif()

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    message("OpenGL found")
endif()

find_package(glm REQUIRED)
if (GLM_FOUND)
    message("glm found")
endif()

find_package(glfw3 REQUIRED)
if (GLFW_FOUND)
    message("glfw found")
endif()

find_package(Freetype REQUIRED)
if (FREETYPE_FOUND)
    message("freetype found")
elseif (WIN32)
    set(FREETYPE_INCLUDE_DIRS C:/lib/freetype/include)
    set(FREETYPE_LIBRARY C:/lib/freetype/lib/freetyped.lib)
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message("glew found")
endif()

find_package(pugixml REQUIRED)
if (PUGIXML_FOUND)
    message("pugixml found")
endif()

find_package(EnTT REQUIRED)
if (ENTT_FOUND)
    message("entt found")
endif()

if (WIN32)
    set(GLM_INCLUDE_DIRS ${glm_DIR}/../)
    set(GLEW_INCLUDE_DIRS ${GLEW_DIR}/../../../include)
    set(ENTT_INCLUDE_DIRS ${EnTT_DIR}/../../../include)
    set(GLEW_LIBRARIES ${GLEW_DIR}/../../glew32d.lib)

    if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/glew32d.dll)
        configure_file(${GLEW_DIR}/../../../bin/glew32d.dll ${CMAKE_CURRENT_BINARY_DIR}/glew32d.dll COPYONLY)
    endif()

    if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/glfw3.dll)
        configure_file(${glfw3_DIR}/../../../bin/glfw3.dll ${CMAKE_CURRENT_BINARY_DIR}/glfw3.dll COPYONLY)
    endif()
endif()

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(cityBuilderGame ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${GLEW_INCLUDE_DIRS}
    ${ENTT_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS})


target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glfw ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${FREETYPE_LIBRARY}
    pugixml
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

