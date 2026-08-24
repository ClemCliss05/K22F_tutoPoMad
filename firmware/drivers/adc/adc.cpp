#include "adc.hpp"
#include "MK22FN512.h"

void Drivers::Adc::init(){
    // Enable GPIOC clock
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

	// Configure pin PTB0 as analog
	PORTB->PCR[0] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[0] |= PORT_PCR_MUX(0x00);

	// Enable ADC clock
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

	// Reset ADC configuration
	ADC0->CFG1    = 0x00000000;
	ADC0->CFG2 	  = 0x00000000;
	ADC0->SC1[0]  = 0x00000000;
	ADC0->SC2     = 0x00000000;
	ADC0->SC3     = 0x00000000;

	// Enable continuous conversion mode
	ADC0->SC3 |= ADC_SC3_ADCO_MASK | ADC_SC3_AVGE_MASK;

	// 12-bit resolution
	ADC0->CFG1 |= ADC_CFG1_MODE(0x01);

	// Select ADC clock source (bus clock)
	ADC0->CFG1 |= ADC_CFG1_ADICLK(0x00);

	// Divide ADC clock
    // ADIV = /2
	ADC0->CFG1 |= ADC_CFG1_ADIV(0x01);

	// Long sample time
    ADC0->CFG1 |= ADC_CFG1_ADLSMP_MASK;

	// Sampling time
    ADC0->CFG2 |= ADC_CFG2_ADLSTS(0x00);

	// Select channel 8
	ADC0->SC1[0] |= ADC_SC1_ADCH(0x08);
}

uint16_t Drivers::Adc::read(){
    // Wait until conversion is complete
	while ((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);

	// Return conversion result
	return ADC0->R[0];
}