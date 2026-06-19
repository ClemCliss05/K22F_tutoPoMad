# ------------------------------------------------------------------------------
# ARM GCC toolchain configuration
#
# This file tells CMake how to cross-compile the project for ARM Cortex-M
# instead of the host machine.
# ------------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

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
set(CMAKE_SIZE    ${TOOLCHAIN_PREFIX}size)

# ------------------------------------------------------------------------------
# Avoid execution tests during configuration
# Embedded binaries cannot run on host machine
# ------------------------------------------------------------------------------

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ------------------------------------------------------------------------------
# CPU flags (Cortex-M0)
# ------------------------------------------------------------------------------

set(CPU_FLAGS "-mcpu=cortex-m0 -mthumb")

# ------------------------------------------------------------------------------
# Default compiler flags
# These are minimal; project targets add their own flags
# ------------------------------------------------------------------------------

set(CMAKE_C_FLAGS_INIT "${CPU_FLAGS} -ffreestanding")
set(CMAKE_CXX_FLAGS_INIT "${CPU_FLAGS} -ffreestanding -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS_INIT "${CPU_FLAGS}")