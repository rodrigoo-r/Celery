# ============================================================================ #
# Release Mode Configuration
# This CMake script configures the build system for release mode.
# ============================================================================ #

if (${RELEASE_MODE})
    message(DEBUG "Release mode enabled. Enabling optimizations.")

    # Set -O3
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()