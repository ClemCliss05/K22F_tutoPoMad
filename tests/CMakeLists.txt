# ------------------------------------------------------------------------------
# Host unit tests
#
# These tests run on the host machine (x86), not on the MCU target.
# They validate platform-independent modules such as:
#   - core utilities
#   - services
#   - algorithms
#
# GoogleTest is downloaded automatically using FetchContent.
# ------------------------------------------------------------------------------

include(FetchContent)

# ------------------------------------------------------------------------------
# Download GoogleTest automatically during configuration
# ------------------------------------------------------------------------------

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

# ------------------------------------------------------------------------------
# Test executable
# ------------------------------------------------------------------------------

add_executable(test_core

    core/test_logger.cpp
    core/test_ringbuffer.cpp
)

# ------------------------------------------------------------------------------
# Link test executable with:
#   - tested module
#   - GoogleTest framework
# ------------------------------------------------------------------------------

target_link_libraries(test_core PRIVATE

    core
    GTest::gtest_main
)

# ------------------------------------------------------------------------------
# Enable AddressSanitizer and UndefinedBehaviorSanitizer
#
# These tools detect:
#   - memory corruption
#   - use-after-free
#   - undefined behavior
# ------------------------------------------------------------------------------

target_compile_options(test_core PRIVATE -fsanitize=address,undefined)
target_link_options(test_core PRIVATE -fsanitize=address,undefined)

# ------------------------------------------------------------------------------
# Register tests automatically with CTest
# ------------------------------------------------------------------------------

include(GoogleTest)
gtest_discover_tests(test_core)