cmake_minimum_required(VERSION 3.16)

# Project name
project(Yuno)

# Set the path to GLFW configuration
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Libraries/lib")

# Include directories for headers
include_directories("${CMAKE_SOURCE_DIR}/Libraries/include")

# Add the executable
add_executable(app glad.c main.cpp)

# Link GLFW to the executable
target_link_libraries(app "${CMAKE_SOURCE_DIR}/Libraries/lib/glfw3.lib")


