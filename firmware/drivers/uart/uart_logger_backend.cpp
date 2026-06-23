#include "uart_logger_backend.hpp"
#include "uart.hpp"

void Drivers::Uart::UartLoggerBackend::write(char c) {
    Drivers::Uart::BSP_Uart_Write_Byte(c);
}