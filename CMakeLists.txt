# ============================================================================ #
# Celery - A Modern C++ High-Performance STL-like Library
# ============================================================================ #

cmake_minimum_required(VERSION 4.0)
project(Celery)

# Set C++20
set(CMAKE_CXX_STANDARD 20)

# Include modular CMake files
include(cmake/deps.cmake)
include(cmake/source.cmake)
include(cmake/link.cmake)

# Set -O3
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Alias
add_library(Celery::Celery ALIAS Celery)