cmake_minimum_required(VERSION 3.16)

project(GLEngine)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB SOURCES src/*cpp)
file(GLOB HEADERS include/*h)
file(GLOB SHADERS shaders/*fs shaders/*vs)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${SHADERS})

# OpenGL
find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)    
    message(STATUS "OpenGL found, including and linking")
    include_directories(${OPENGL_INCLUDE_DIR})
    link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
endif()

# GLEW
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    message(STATUS "GLEW found, including and linking")
    target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
endif()

# GLFW3
find_package(GLFW3 REQUIRED)
if(GLFW3_FOUND)
    message(STATUS "GLFW3 found, including and linking")
    target_link_libraries(${PROJECT_NAME} glfw)
endif()
