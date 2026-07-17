#include "uart.hpp"
#include "MK22FN512.h"

/*
 * BSP_Uart_Init()
 * For core / system clock of 20MHz
 * USART1 @ 115200 Full Duplex
 * 1 start - 8-bit - 1 stop
 * TX -> PE0 (ALT3)
 * RX -> PE1 (ALT3)
 */

void Drivers::Uart::init() {

    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; // Enable clock for PORTE
    SIM->SCGC4 |= SIM_SCGC4_UART1_MASK; // Enable clock for UART1

    // Set PE0 & PE1 to ALT3 (UART1_TX and UART1_RX)
    PORTE->PCR[0] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[0] |= PORT_PCR_MUX(3);
    PORTE->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[1] |= PORT_PCR_MUX(3);

    // UART1 configuration: 8 data bits, no parity, 1 stop bit
    UART1->C1 = 0; // Clear C1 register for default settings
    UART1->C2 = 0; // Disable transmitter and receiver
    UART1->C3 = 0; // Error interupts disabled

    // Baud Rate = 115200
    // UART baud rate = UART module clock / (16 × (SBR[12:0] + BRFD))

    // With UARTck=48MHz, USARTDIV =   48E6/(16*115200) = 26,04167
    // BDHsbr = 00000 | BDLsbr = 00011010 = 26 -> Baud Rate = 115 384,61538
    // Error (%) = ((BRobtained - BRwanted) / BR wanted) * 100
    // Error = ((115 384,61538 - 115200) / 115 384,61538) * 100 = 0.16 %
    // BRFD = (UARTclk / (Uartbd*16)) - SBR
    // BRFD = (48MHz/(16*115200))-26 = 0,04167
    // Then BRFD = 0b00001 = 1/32 = 0.03125 (see table 47-9) to reduce error
    // Finally, Baud Rate = 48e6 / (16*(26 + 0.03125)) = 115 246,09844 -> 0.04 %

    // With UARTck=120MHz, USARTDIV =   120E6/(16*115200) = 65,10416
    // BDHsbr = 00000 | BDLsbr = 01000001 -> Baud Rate = 115 384,61538 -> 0.16 % error
    // With BRFD = 0b00011 = 3/32 = 0.09375 -> Baud Rate = 115 218,4349 -> 0.02 % error

    UART1->BDH &= ~UART_BDH_SBR_MASK;
    UART1->BDL &= ~UART_BDL_SBR_MASK;
    UART1->BDL |= UART_BDL_SBR(0x1A);
    UART1->C4 &= ~UART_C4_BRFA_MASK;
    UART1->C4 |= UART_C4_BRFA(0b00001);

    // Enable RE and TE
    UART1->C2 |= UART_C2_RE_MASK | UART_C2_TE_MASK;
}

void Drivers::Uart::writeByte(uint8_t c) {

    // Wait until the transmit buffer is empty
    while ((UART1->S1 & UART_S1_TDRE_MASK) != UART_S1_TDRE_MASK);
		UART1->D = c; // Write the byte to the data register
}

void Drivers::Uart::write(const char *str) {

    while (*str != '\0')
    {
        writeByte(*str++);
    }
}