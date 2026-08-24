# ------------------------------------------------------------------------------
# ARM GCC toolchain configuration
#
# This file tells CMake how to cross-compile the project for ARM Cortex-M
# instead of the host machine.
# ------------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
# ------------------------------------------------------------------------------
# CPU configuration
# IMPORTANT:
# This value must match the Cortex-M CPU of the target MCU.
# Change this value when adapting this platform to another MCU.
# ------------------------------------------------------------------------------
set(PLATFORM_CPU_FLAGS "-mcpu=cortex-m4 -mthumb")

# ------------------------------------------------------------------------------
# Toolchain prefix
# ------------------------------------------------------------------------------

set(TOOLCHAIN_PREFIX arm-none-eabi-)

# ------------------------------------------------------------------------------
# Compilers
# ------------------------------------------------------------------------------

set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)

# ------------------------------------------------------------------------------
# Binary utilities
# ------------------------------------------------------------------------------

set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump)
set(CMAKE_SIZE    ${TOOLCHAIN_PREFIX}size)

# ------------------------------------------------------------------------------
# Avoid execution tests during configuration
# Embedded binaries cannot run on host machine
# ------------------------------------------------------------------------------

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ------------------------------------------------------------------------------
# Default compiler flags
#
# CPU-specific flags are defined by the selected platform.
# ------------------------------------------------------------------------------

set(CMAKE_C_FLAGS_INIT "${PLATFORM_CPU_FLAGS} -ffreestanding")
set(CMAKE_CXX_FLAGS_INIT "${PLATFORM_CPU_FLAGS} -ffreestanding -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS_INIT "${PLATFORM_CPU_FLAGS}")