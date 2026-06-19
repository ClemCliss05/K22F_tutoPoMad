# ------------------------------------------------------------------------------
# Services layer
# High level modules built on top of drivers
# ------------------------------------------------------------------------------

add_library(services STATIC)

target_sources(services PRIVATE
    sensor/sensor.cpp
)

target_include_directories(services PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/sensor
)

target_link_libraries(services PUBLIC

    core
)
