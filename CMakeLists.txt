# ============================================================================ #
# Celery - A Modern C++ High-Performance STL-like Library
# ============================================================================ #

cmake_minimum_required(VERSION 4.0)
project(Celery)

# Set C++20
set(CMAKE_CXX_STANDARD 20)

# Include modular CMake files
include(cmake/source.cmake)
include(cmake/link.cmake)
include(cmake/release.cmake)

# Alias
add_library(Celery::Celery ALIAS Celery)