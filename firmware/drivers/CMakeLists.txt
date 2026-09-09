# ------------------------------------------------------------------------------
# Hardware Drivers library
# Direct register access (no HAL)
# ------------------------------------------------------------------------------

add_library(drivers STATIC)

target_sources(drivers PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/gpio/gpio.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/uart/uart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uart/uart_logger_backend.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/adc/adc.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/pit/pit.cpp
)

target_include_directories(drivers PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/gpio

    ${CMAKE_CURRENT_SOURCE_DIR}/uart
    
    ${CMAKE_CURRENT_SOURCE_DIR}/adc
    
    ${CMAKE_CURRENT_SOURCE_DIR}/pit
)

target_link_libraries(drivers PUBLIC

    core
    platform_mk22fn512
)

target_compile_definitions(drivers PUBLIC

    MK22FN512
)