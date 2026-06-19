cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME firmwareTemplate)
project(${PROJECT_NAME} C CXX ASM)

# ------------------------------------------------------------------------------
# Language standards
# ------------------------------------------------------------------------------

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# Export compile_commands.json (useful for clangd / clang-tidy)
# ------------------------------------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------------------
# Default build type
# ------------------------------------------------------------------------------

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# ------------------------------------------------------------------------------
# Option to build only hardware independent firmware parts and tests
# ------------------------------------------------------------------------------

option(ANALYSIS "Build hardware independent firmware parts for analysis" OFF)
option(TESTS "Enable ASAN/UBSAN and tests" OFF)

# ------------------------------------------------------------------------------
# Host tests
# ------------------------------------------------------------------------------

if(ANALYSIS)
    add_subdirectory(firmware/core)
    add_subdirectory(firmware/services)
    if(TESTS)
        enable_testing()
        add_subdirectory(tests)
    endif()
else()
    add_subdirectory(firmware)
endif()