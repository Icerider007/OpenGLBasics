cmake_minimum_required(VERSION 3.16)

# Project name
project(Yuno)

#Adding the glfw directory
add_subdirectory(Libraries/glfw)

# Include directories for headers
include_directories("${CMAKE_SOURCE_DIR}/Libraries/include")

# Add the executable
add_executable(app glad.c main.cpp)

# Link GLFW to the executable
target_link_libraries(app PRIVATE glfw)


