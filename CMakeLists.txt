cmake_minimum_required(VERSION 3.16)

# Project name
project(Yuno)

#Adding the glfw directory
add_subdirectory(Libraries/glfw)
#adding stbi subdir
include_directories("${CMAKE_SOURCE_DIR}/Libraries/include/stb")

# Include directories for headers
include_directories("${CMAKE_SOURCE_DIR}/Libraries/include/")

# Add the executable
add_executable(app main.cpp glad.c shaderClass.cpp stb.cpp)

# Link GLFW to the executable
target_link_libraries(app PRIVATE glfw)


