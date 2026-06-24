#include "gpio.hpp"
#include "MK22FN512.h"

/*
 * LED driver functions
 */

void Drivers::Gpio::LED_Init() {
    // Enable clock for PORTA & PORTD
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK;

    // Set PTA1 PTA2 and PTD5 to GPIO mode
    PORTA->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[1] |= PORT_PCR_MUX(0x01);
    PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[2] |= PORT_PCR_MUX(0x01);
    PORTD->PCR[5] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[5] |= PORT_PCR_MUX(0x01);

    // Set PTA1 PTA2 and PTD5 as output
    GPIOA->PDDR |= (0b1 << 1);
    GPIOA->PDDR |= (0b1 << 2);
    GPIOD->PDDR |= (0b1 << 5);

    // Set PTA1 PTA2 and PTD5 off
    GPIOA->PSOR = (0b1 << 1);
    GPIOA->PSOR = (0b1 << 2);
    GPIOD->PSOR = (0b1 << 5);
}

void Drivers::Gpio::LED_On(void) {
    GPIOA->PCOR = (0b1 << 1);
}

void Drivers::Gpio::LED_On(uint8_t color) {
    switch (color) {
    case LED_RED:
        GPIOA->PCOR = (0b1 << 1);
        break;
    case LED_GREEN:
        GPIOA->PCOR = (0b1 << 2);
        break;
    case LED_BLUE:
        GPIOD->PCOR = (0b1 << 5);
        break;
    case LED_CYAN:
        GPIOA->PCOR = (0b1 << 2);
        GPIOD->PCOR = (0b1 << 5);
        break;
    case LED_YELLOW:
        GPIOA->PCOR = (0b1 << 1);
        GPIOA->PCOR = (0b1 << 2);
        break;
    case LED_MAGENTA:
        GPIOA->PCOR = (0b1 << 1);
        GPIOD->PCOR = (0b1 << 5);
        break;
    default:
        GPIOA->PCOR = (0b1 << 1);
        GPIOA->PCOR = (0b1 << 2);
        GPIOD->PCOR = (0b1 << 5);
        break;
    }
}

void Drivers::Gpio::LED_Off(void) {
    GPIOA->PSOR = (0b1 << 1);
}

void Drivers::Gpio::LED_Off(uint8_t color) {
    switch (color) {
    case LED_RED:
        GPIOA->PSOR = (0b1 << 1);
        break;
    case LED_GREEN:
        GPIOA->PSOR = (0b1 << 2);
        break;
    case LED_BLUE:
        GPIOD->PSOR = (0b1 << 5);
        break;
    case LED_CYAN:
        GPIOA->PSOR = (0b1 << 2);
        GPIOD->PSOR = (0b1 << 5);
        break;
    case LED_YELLOW:
        GPIOA->PSOR = (0b1 << 1);
        GPIOA->PSOR = (0b1 << 2);
        break;
    case LED_MAGENTA:
        GPIOA->PSOR = (0b1 << 1);
        GPIOD->PSOR = (0b1 << 5);
        break;
    default:
        GPIOA->PSOR = (0b1 << 1);
        GPIOA->PSOR = (0b1 << 2);
        GPIOD->PSOR = (0b1 << 5);
        break;
    }
}

void Drivers::Gpio::LED_Toggle(void) {
    GPIOA->PTOR = (0b1 << 1);
}

void Drivers::Gpio::LED_Toggle(uint8_t color) {
    switch (color) {
    case LED_RED:
        GPIOA->PTOR = (0b1 << 1);
        break;
    case LED_GREEN:
        GPIOA->PTOR = (0b1 << 2);
        break;
    case LED_BLUE:
        GPIOD->PTOR = (0b1 << 5);
        break;
    case LED_CYAN:
        GPIOA->PTOR = (0b1 << 2);
        GPIOD->PTOR = (0b1 << 5);
        break;
    case LED_YELLOW:
        GPIOA->PTOR = (0b1 << 1);
        GPIOA->PTOR = (0b1 << 2);
        break;
    case LED_MAGENTA:
        GPIOA->PTOR = (0b1 << 1);
        GPIOD->PTOR = (0b1 << 5);
        break;
    default:
        GPIOA->PTOR = (0b1 << 1);
        GPIOA->PTOR = (0b1 << 2);
        GPIOD->PTOR = (0b1 << 5);
        break;
    }
}

/*
 * Push Button driver functions
 */

void Drivers::Gpio::PBs_Init(void) {
    // Enable clock for PORTC
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    // Set PTC1 to GPIO mode
    PORTC->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[1] |= PORT_PCR_MUX(0x01);

    // Set PTB17 to GPIO mode
    PORTB->PCR[17] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[17] |= PORT_PCR_MUX(0x01);

    // Set PTC1 as input
    GPIOC->PDDR &= ~(0b1 << 1);
    // Set PTB17 as input
    GPIOB->PDDR &= ~(0b1 << 17);
}

bool Drivers::Gpio::PB1_GetState(void) {
    // Return true if button is pressed (PTC1 is low)
    return !(GPIOC->PDIR & (0b1 << 1));
}

bool Drivers::Gpio::PB2_GetState(void) {
    // Return true if button is pressed (PTB17 is low)
    return !(GPIOB->PDIR & (0b1 << 17));
}