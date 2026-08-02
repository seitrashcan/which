# This file is part of a project that is licensed under Apache-2.0.
# See `LICENSE` for details.

cmake_minimum_required(VERSION 3.20)

project(which LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(which INTERFACE)
add_library(mhx::which ALIAS which)

target_include_directories(which INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(which INTERFACE cxx_std_23)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(WHICH_BUILD_TESTS "Build the which test suite" ON)
else()
    option(WHICH_BUILD_TESTS "Build the which test suite" OFF)
endif()

if(WHICH_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
