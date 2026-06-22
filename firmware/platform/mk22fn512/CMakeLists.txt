# ------------------------------------------------------------------------------
# MK22FN512 platform library
#
# This library contains:
#   - CMSIS headers
#   - startup code
#   - linker script
#   - MCU specific configuration
# ------------------------------------------------------------------------------

add_library(platform_mk22fn512 STATIC)

# ------------------------------------------------------------------------------
# Source files
# ------------------------------------------------------------------------------

file(GLOB CMSIS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/device/src/*.c
)

target_sources(platform_mk22fn512 PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/platform_drivers/clock/clock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform_drivers/interrupt/interrupt.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/startup/startup_MK22FN512.cpp

    ${CMSIS_SOURCES}
)

# ------------------------------------------------------------------------------
# Include directories
# ------------------------------------------------------------------------------

target_include_directories(platform_mk22fn512 PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/platform_drivers/clock
    ${CMAKE_CURRENT_SOURCE_DIR}/platform_drivers/interrupt

    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/core
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/device/inc
)

# ------------------------------------------------------------------------------
# Preprocessor definitions
# ------------------------------------------------------------------------------

target_compile_definitions(platform_mk22fn512 PUBLIC
    MK22FN512
)

# ------------------------------------------------------------------------------
# Compiler flags
# ------------------------------------------------------------------------------

target_compile_options(platform_mk22fn512 PUBLIC

    -Wall
    -Wextra
    -Wpedantic

    -ffunction-sections
    -fdata-sections
)

# ------------------------------------------------------------------------------
# Linker script
# ------------------------------------------------------------------------------

set(LINKER_SCRIPT
    ${CMAKE_CURRENT_SOURCE_DIR}/linker/linkerscript.ld
)

target_link_options(platform_mk22fn512 PUBLIC

    -T${LINKER_SCRIPT}

    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map

    --specs=nano.specs
    --specs=nosys.specs
)
