# Minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Project name
project(TarLib)

# Add the option TAR_TEST with default value OFF
option(TAR_TEST "Enable TAR_TEST" ON)

add_subdirectory(lib)

if(TAR_TEST)
    add_executable(
        libtest
        test/test.cpp
        lib/include/tarlib.hpp
        lib/src/tarlib.cpp
        )
    target_link_libraries(libtest tarlib)
endif()