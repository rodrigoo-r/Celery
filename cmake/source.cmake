# ============================================================================ #
# Celery - A Modern C++ High-Performance STL-like Library
# ============================================================================ #

# Find all files
file(GLOB_RECURSE CELERY_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/celery/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/celery/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Add executable target
add_library(Celery INTERFACE ${CELERY_SOURCES})
add_library(Celery::Celery ALIAS Celery)