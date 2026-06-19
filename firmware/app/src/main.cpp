#include "MK22FN512.h"

int main()
{
    // Enable clock for PORTA & PORTD
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK;

    // Set PTA1 PTA2 and PTD5 to GPIO mode
    PORTA->PCR[1] = PORT_PCR_MUX(0x01);
    PORTA->PCR[2] = PORT_PCR_MUX(0x01);
    PORTD->PCR[5] = PORT_PCR_MUX(0x01);

    // Set PTA1 PTA2 and PTD5 as output
    GPIOA->PDDR |= (1u << 1);
    GPIOA->PDDR |= (1u << 2);
    GPIOD->PDDR |= (1u << 5);

    // Set PTA1 PTA2 and PTD5 off
    GPIOA->PSOR = (1u << 1);
    GPIOA->PSOR = (1u << 2);
    GPIOD->PSOR = (1u << 5);

    while (1)
    {
        // Red
        GPIOA->PCOR = (1u << 1);
        GPIOD->PSOR = (1u << 5);

        for (volatile uint32_t i = 0; i < 5000000; i++);

        // Green
        GPIOA->PCOR = (1u << 2);
        GPIOA->PSOR = (1u << 1);

        for (volatile uint32_t i = 0; i < 5000000; i++);

        // Blue
        GPIOD->PCOR = (1u << 5);
        GPIOA->PSOR = (1u << 2);

        for (volatile uint32_t i = 0; i < 5000000; i++);
    }
}