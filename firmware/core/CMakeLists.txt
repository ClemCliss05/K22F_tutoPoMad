# ------------------------------------------------------------------------------
# Core firmware utilities
# Platform independent code
# ------------------------------------------------------------------------------

add_library(core STATIC)

target_sources(core PRIVATE

    logger/logger.cpp
    ringbuffer/ringbuffer.cpp
)

target_include_directories(core PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/logger
    ${CMAKE_CURRENT_SOURCE_DIR}/ringbuffer
)

target_compile_options(core PRIVATE

    -Wall
    -Wextra
    -Wpedantic
)
