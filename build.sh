# ============================================================================ #
# Build script for the Celery project using CMake.
# ============================================================================ #

cmake -DCMAKE_BUILD_TYPE=Release --build cmake-build-debug --target Celery -j 8