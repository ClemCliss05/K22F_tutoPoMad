# ------------------------------------------------------------------------------
# Services library
#
# Application-level services built on top of the core and drivers.
# ------------------------------------------------------------------------------

add_library(services STATIC)

target_sources(services PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/sensor/sensor.cpp
)

target_include_directories(services PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/sensor
)

target_link_libraries(services PUBLIC

    core
)