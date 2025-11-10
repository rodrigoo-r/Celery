# ============================================================================ #
# Fetches and includes all source files for the project.
# ============================================================================ #

file(GLOB_RECURSE ALL_CPP "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE ALL_H   "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(ALL_FILES ${ALL_CPP} ${ALL_H})

# Exclude generated build files
foreach(file ${ALL_FILES})
    if(file MATCHES "${CMAKE_BINARY_DIR}")
        list(REMOVE_ITEM ALL_FILES ${file})
    endif()
endforeach()

add_library(${PROJECT_NAME} STATIC ${ALL_FILES})