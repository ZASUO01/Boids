cmake_minimum_required(VERSION 4.0)
set(CMAKE_C_STANDARD 11)

set(PROJECT_NAME "Boids")
project(${PROJECT_NAME})

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW SDL2::SDL2 OpenGL::GL)