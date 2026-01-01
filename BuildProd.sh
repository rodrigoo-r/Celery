# ============================================================================ #
# Build script for the Celery project using CMake.
# ============================================================================ #

cmake -DCMAKE_BUILD_TYPE=Release -S . -B cmake-build-release
cmake --build cmake-build-release --target Celery -j 8