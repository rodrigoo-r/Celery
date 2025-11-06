# ============================================================================ #
# Release Mode Configuration
# This CMake script configures the build system for release mode.
# ============================================================================ #

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(DEBUG "Release mode enabled. Enabling optimizations.")

    # Set -O3
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

    # Enable Link Time Optimization (LTO) if supported
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()