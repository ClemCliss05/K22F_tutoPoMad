# ------------------------------------------------------------------------------
# Services library
#
# Application-level services built on top of the core and drivers.
# ------------------------------------------------------------------------------

add_library(services STATIC)

target_sources(services PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/sensor/sensor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/delay/delay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/uart_logger_backend.cpp
)

target_include_directories(services PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/sensor
    ${CMAKE_CURRENT_SOURCE_DIR}/delay
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/
)

target_link_libraries(services PUBLIC

    core
    drivers
)