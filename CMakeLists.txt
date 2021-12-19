cmake_minimum_required(VERSION 3.0.0)
project(trafficSimulation VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(./include/)
include(sourcelist.cmake)

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

add_executable(trafficSimulation ${SOURCES})

target_include_directories(trafficSimulation PUBLIC
    /usr/include/entt
    ${FREETYPE_INCLUDE_DIRS})

target_link_libraries(trafficSimulation
    ${OPENGL_LIBRARIES}
    glfw ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}   
    ${FREETYPE_LIBRARIES} 
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

