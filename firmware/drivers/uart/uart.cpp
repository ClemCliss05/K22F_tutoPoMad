#include "uart.hpp"
#include "MK22FN512.h"

/*
 * BSP_Uart_Init()
 * USART1 @ 115200 Full Duplex
 * 1 start - 8-bit - 1 stop
 * TX -> PE0 (ALT3)
 * RX -> PE1 (ALT3)
 */

void Drivers::Uart::BSP_Uart_Init() {


    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; // Enable clock for PORTE
    PORTE->PCR[0] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[0] |= PORT_PCR_MUX(3); // Set PE0 to ALT3 (UART1_TX)
    PORTE->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[1] |= PORT_PCR_MUX(3); // Set PE1 to ALT3 (UART1_RX)


    SIM->SCGC4 |= SIM_SCGC4_UART1_MASK; // Enable clock for UART1

}

void Drivers::Uart::BSP_Uart_Write_Byte(char c) {
    while (1) {
        // Wait until transmit data register is empty
    }

    // Write the byte to the transmit data register
}