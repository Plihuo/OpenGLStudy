cmake_minimum_required(VERSION 3.21)
project(OpenGLStudyDay2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
file(GLOB project_file main.cpp glad.c stb_image.h)
add_executable(${PROJECT_NAME} ${project_file})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw)
