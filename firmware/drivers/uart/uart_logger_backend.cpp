#include "uart_logger_backend.hpp"

UartLoggerBackend::UartLoggerBackend(Drivers::Uart &uart) : uart_(uart) {}

void UartLoggerBackend::write(char c) {
    uart_.writeByte(static_cast<uint8_t>(c));
}