#include "uart_logger_backend.hpp"
#include "uart.hpp"

void UartLoggerBackend::write(char c) {
    uart_write_byte(c);
}