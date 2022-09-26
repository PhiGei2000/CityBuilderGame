cmake_minimum_required(VERSION 3.20.0)
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
file(GLOB RECOURSE)

if(WIN32)
    include(./variablesWin.cmake)

    message("${CMAKE_PREFIX_PATH}")    
endif()

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
find_package(pugixml REQUIRED)


file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(cityBuilderGame ${SOURCES})

if(WIN32)
    find_package(EnTT REQUIRED)

    target_include_directories(${PROJECT_NAME} PUBLIC 
        ${GLEW_INCLUDE_DIRS}
        ${ENTT_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${STB_INCLUDE_DIRS})
else()
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${ENTT_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS})
endif()


target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glfw ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    pugixml    
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

