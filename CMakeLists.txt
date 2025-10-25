cmake_minimum_required(VERSION 4.0)
project(Celery)

set(CMAKE_CXX_STANDARD 20)

# Include modular CMake files
include(cmake/deps.cmake)
include(cmake/source.cmake)
include(cmake/link.cmake)

# Alias
add_library(Celery::Celery ALIAS Celery)