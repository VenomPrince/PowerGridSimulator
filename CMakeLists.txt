cmake_minimum_required(VERSION 3.10)
project(PowerGridSimulator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory for header files
include_directories(include)

# Add all source files
set(SOURCES
    src/main.cpp
    src/Load.cpp
    src/PowerSource.cpp
    src/Busbar.cpp
    src/Grid.cpp
    src/Simulator.cpp
)

# Create executable
add_executable(PowerGridSimulator ${SOURCES})

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
