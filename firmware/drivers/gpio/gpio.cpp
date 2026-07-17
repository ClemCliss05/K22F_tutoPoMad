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

void Drivers::Gpio::LED_On(LedColor color) {
    switch (color) {
    case Drivers::Gpio::LedColor::Red:
        GPIOA->PCOR = (0b1 << 1);
        break;
    case Drivers::Gpio::LedColor::Green:
        GPIOA->PCOR = (0b1 << 2);
        break;
    case Drivers::Gpio::LedColor::Blue:
        GPIOD->PCOR = (0b1 << 5);
        break;
    case Drivers::Gpio::LedColor::Cyan:
        GPIOA->PCOR = (0b1 << 2);
        GPIOD->PCOR = (0b1 << 5);
        break;
    case Drivers::Gpio::LedColor::Yellow:
        GPIOA->PCOR = (0b1 << 1);
        GPIOA->PCOR = (0b1 << 2);
        break;
    case Drivers::Gpio::LedColor::Magenta:
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

void Drivers::Gpio::LED_Off(LedColor color) {
    switch (color) {
    case Drivers::Gpio::LedColor::Red:
        GPIOA->PSOR = (0b1 << 1);
        break;
    case Drivers::Gpio::LedColor::Green:
        GPIOA->PSOR = (0b1 << 2);
        break;
    case Drivers::Gpio::LedColor::Blue:
        GPIOD->PSOR = (0b1 << 5);
        break;
    case Drivers::Gpio::LedColor::Cyan:
        GPIOA->PSOR = (0b1 << 2);
        GPIOD->PSOR = (0b1 << 5);
        break;
    case Drivers::Gpio::LedColor::Yellow:
        GPIOA->PSOR = (0b1 << 1);
        GPIOA->PSOR = (0b1 << 2);
        break;
    case Drivers::Gpio::LedColor::Magenta:
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

void Drivers::Gpio::LED_Toggle(LedColor color) {
    switch (color) {
    case Drivers::Gpio::LedColor::Red:
        GPIOA->PTOR = (0b1 << 1);
        break;
    case Drivers::Gpio::LedColor::Green:
        GPIOA->PTOR = (0b1 << 2);
        break;
    case Drivers::Gpio::LedColor::Blue:
        GPIOD->PTOR = (0b1 << 5);
        break;
    case Drivers::Gpio::LedColor::Cyan:
        GPIOA->PTOR = (0b1 << 2);
        GPIOD->PTOR = (0b1 << 5);
        break;
    case Drivers::Gpio::LedColor::Yellow:
        GPIOA->PTOR = (0b1 << 1);
        GPIOA->PTOR = (0b1 << 2);
        break;
    case Drivers::Gpio::LedColor::Magenta:
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