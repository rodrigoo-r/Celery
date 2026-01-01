# ============================================================================ #
# Links all necessary libraries and sets up the build environment.
# ============================================================================ #

# Link libraries
target_include_directories(
        Celery INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)