# Minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Project name
project(TarLib)
# FILEPATH: /d:/tarlib/lib/CMakeLists.txt

# Define the static library target
add_library(tarlib
    STATIC src/tarlib.cpp
    )

# Set the include directories for the library
target_include_directories(tarlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set additional properties for the library
set_target_properties(tarlib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
