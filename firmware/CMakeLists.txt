# ------------------------------------------------------------------------------
# Firmware modules
# ------------------------------------------------------------------------------

add_subdirectory(core)
add_subdirectory(services)

# Build hardware dependent modules if tests not enabled
if(NOT ENABLE_TESTS)
    add_subdirectory(drivers)
    add_subdirectory(platform)
    add_subdirectory(app)
endif()