# -----------------------------------------------------------------------------
# Project
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.19)
project(estd CXX)

set(PROJECT_COMPILE_FEATURES cxx_std_17)
set(PROJECT_COMPILE_OPTIONS -Wextra -Wno-user-defined-literals)
set(PROJECT_COMPILE_DEFINES)
set(PROJECT_LINK_LIBRARIES)

set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src)
set(PROJECT_INCLUDES)

# -----------------------------------------------------------------------------
# Make Header-Only Library
# -----------------------------------------------------------------------------
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/estd.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_compile_features(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_FEATURES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINES})
target_compile_options(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_OPTIONS})

# -----------------------------------------------------------------------------
# Make Test Application
# -----------------------------------------------------------------------------
# if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
#     add_executable(${PROJECT_NAME}-test ${PROJECT_SOURCE_DIR}/src/test.cpp)
#     target_include_directories(${PROJECT_NAME}-test PUBLIC ${PROJECT_INCLUDE_DIRS})
#     target_compile_features(${PROJECT_NAME}-test PUBLIC ${PROJECT_COMPILE_FEATURES})
#     target_compile_definitions(${PROJECT_NAME}-test PUBLIC ${PROJECT_COMPILE_DEFINES})
#     target_compile_options(${PROJECT_NAME}-test PUBLIC ${PROJECT_COMPILE_OPTIONS})
# endif()
