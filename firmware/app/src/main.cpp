#include "clock.hpp"
#include "gpio.hpp"
#include "adc.hpp"

#include "uart.hpp"
#include "uart_logger_backend.hpp"
#include "logger.hpp"
#include "ringbuffer.hpp"

#include "MK22FN512.h"

void delay(uint32_t amount) {
    for (volatile uint32_t i = 0; i < amount; i++)
    {
        __NOP();
    }
}

int main() {

    // Init Clock
    Bsp::Clock clock;
    clock.initOSC();
    clock.set48MHz();

    // Init GPIO
    Drivers::Gpio gpio;
    gpio.LED_Init();
    gpio.PBs_Init();

    // Init UART
    Drivers::Uart uart;
    uart.init();
    UartLoggerBackend uartBackend(uart);
    char loggerBuffer[128];
    RingBuffer ringBuffer(loggerBuffer, sizeof(loggerBuffer));
    Logger logger(ringBuffer, uartBackend);
    
    uint32_t i = 0;
    
    LOG_DEBUG("Boot");
    LOG_DEBUG("Clock OK");
    LOG_DEBUG("UART OK");

    // Initialize ADC0_SE8 on PTB0
    Drivers::Adc adc;
	adc.init();
	LOG_DEBUG("ADC OK");

    gpio.LED_On();
    delay(5000000);
    gpio.LED_Off();

    while (1) {
        // Wait here until ADC COCO
		while ((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);

		// Report result to console
		LOG_INFO("ADC value = %d\r\n", ADC0->R[0]);

		// Wait about 200ms
		for (i=0; i<500000; i++);
    }
}