#include "uart.hpp"

// Hardware specific implementation
// For now this is a stub used for template validation

void uart_init() {
    // Initialize UART hardware here
}

void uart_write_byte(char c) {
    while (1) {
        // Wait until transmit data register is empty
    }

    // Write the byte to the transmit data register
}