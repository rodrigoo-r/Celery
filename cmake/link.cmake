# ============================================================================ #
# Links all necessary libraries and sets up the build environment.
# ============================================================================ #

# xxHash static library
add_library(xxhash STATIC ${xxhash_SOURCE_DIR}/xxhash.c)
target_include_directories(xxhash PUBLIC ${xxhash_SOURCE_DIR})

# Link libraries
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${xxhash_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        xxhash
)