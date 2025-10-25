# ============================================================================ #
# Fetches all external dependencies using CMake's FetchContent module.
# ============================================================================ #

include(FetchContent)

FetchContent_Declare(
        xxhash
        GIT_REPOSITORY https://github.com/Cyan4973/xxHash.git
        GIT_TAG        v0.8.3
)

FetchContent_MakeAvailable(xxhash)