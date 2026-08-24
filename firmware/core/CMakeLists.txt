# ------------------------------------------------------------------------------
# Core library
#
# Hardware independent firmware components.
# This library must remain independent from drivers and platform code.
# ------------------------------------------------------------------------------

add_library(core STATIC)

target_sources(core PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/logger/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ringbuffer/ringbuffer.cpp
)

target_include_directories(core PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/logger
    ${CMAKE_CURRENT_SOURCE_DIR}/ringbuffer
)