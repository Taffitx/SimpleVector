# Set cmake version
cmake_minimum_required(VERSION 3.11)

# Set the project name
project(SimpleVectorProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable 
add_executable(SimpleVector src/main.cpp)

# Specify the include directories
target_include_directories(SimpleVector PUBLIC include)