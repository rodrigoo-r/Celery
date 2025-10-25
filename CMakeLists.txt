cmake_minimum_required(VERSION 4.0)
project(celery)

set(CMAKE_CXX_STANDARD 20)

# Include modular CMake files
include(cmake/deps.cmake)
include(cmake/source.cmake)
include(cmake/link.cmake)
