# TODO: This Cmake file is a work in progress

# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MyProject)

# Specify the C++ standard to use
set(CMAKE_CXX_STANDARD 11)

# Add an executable target
add_executable(my_program
main.cpp
../PhaseJump/Classes/Core/AlignFunc.h
../PhaseJump/Classes/Core/AlignFunc.cpp
)
