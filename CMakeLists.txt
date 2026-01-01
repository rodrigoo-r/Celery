# ============================================================================ #
# Celery - A Modern C++ High-Performance STL-like Library
# ============================================================================ #

cmake_minimum_required(VERSION 3.20)
project(Celery)

# Set C++20
set(CMAKE_CXX_STANDARD 20)

add_library(Celery INTERFACE)

# Alias
add_library(Celery::Celery ALIAS Celery)

# Include modular CMake files
include(Cmake/Link.cmake)