cmake_minimum_required(VERSION 3.22.1)
set(PROJECT_NAME cityBuilderGame)

project(${PROJECT_NAME} VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 23)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Create DEBUG definition")

    add_compile_definitions(DEBUG)
endif()

include_directories(./include/)

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    message("OpenGL found")
endif()

if(WIN32)
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_LIST_DIR}/lib)

    # install glfw
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(lib/glfw)

    # build glew
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_LIST_DIR}/lib/glew-cmake/build)
    add_subdirectory(lib/glew-cmake)
    set(GLEW_INCLUDE_DIRS lib/glew-cmake/include)

    set(GLEW_LIBRARIES ${CMAKE_CURRENT_BINARY_DIR}/lib/glew-cmake/lib/glew-sharedd.lib)

    if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/lib/glew-cmake/bin/glew-sharedd.dll)
        file(COPY_FILE ${CMAKE_CURRENT_BINARY_DIR}/lib/glew-cmake/bin/glew-sharedd.dll ${CMAKE_CURRENT_BINARY_DIR}/glew-sharedd.dll ONLY_IF_DIFFERENT)
    endif()

    add_subdirectory(lib/pugixml)

    # include entt
    set(ENTT_INCLUDE_DIRS lib/entt/src HINTS ${CMAKE_CURRENT_LIST_DIR})

    # include glm
    set(GLM_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/lib/glm)

    # include stb
    include_directories(${CMAKE_CURRENT_LIST_DIR}/lib/stb)

    # include libnoise
    add_subdirectory(lib/libnoise)
    set(LIBNOISE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/lib/libnoise/src)
    set(LIBNOISE_LIBRARIES noise)

    set(FREETYPE_INCLUDE_DIRS ./lib/freetype/include)
    find_file(FREETYPE_LIBRARY freetype.lib HINTS "${CMAKE_CURRENT_LIST_DIR}/lib/freetype/release static/vs2015-2022/win64")
else()
    find_package(glm REQUIRED)
    if (GLM_FOUND)
        message("glm found")
    endif()

    find_package(glfw3 REQUIRED)
    if (GLFW_FOUND)
        message("glfw found")
    endif()

    find_package(pugixml REQUIRED)
    if (PUGIXML_FOUND)
        message("pugixml found")
    endif()

    find_package(EnTT REQUIRED)
    if (ENTT_FOUND)
        message("entt found")
    endif()

    find_package(GLEW REQUIRED)
    if (GLEW_FOUND)
        message("glew found")
    endif()

    set(LIBNOISE_LIBRARIES libnoise.so)

    include_directories(/usr/include/freetype2)
    include_directories(/usr/include/stb)

    set(FREETYPE_LIBRARY libfreetype.so)
endif()


file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(cityBuilderGame ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${GLEW_INCLUDE_DIRS}
    ${ENTT_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${LIBNOISE_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glfw ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${FREETYPE_LIBRARY}
    pugixml
    ${LIBNOISE_LIBRARIES}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(RESOURCE_PATH ${CMAKE_CURRENT_LIST_DIR}/res)
if (WIN32)
    # copy resource directory to build dir
    file(CREATE_LINK ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res SYMBOLIC)
endif()
