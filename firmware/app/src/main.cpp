#include "clock.hpp"
#include "gpio.hpp"
#include "pit.hpp"

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
    
    LOG_DEBUG("Boot");
    LOG_DEBUG("Clock OK");
    LOG_DEBUG("UART OK");

    // Initialize PIT channel[0]
    Drivers::Pit pit;
	pit.init();
    volatile uint32_t loop = 0;
	LOG_DEBUG("PIT OK");

    gpio.LED_On();
    delay(5000000);
    gpio.LED_Off();

    while (1) {
        volatile uint32_t cval = PIT->CHANNEL[0].CVAL;
        volatile uint32_t tflg = PIT->CHANNEL[0].TFLG;

        if(PIT->CHANNEL[0].TFLG == PIT_TFLG_TIF_MASK){
            PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK; // clear TIF flag
            tflg = PIT->CHANNEL[0].TFLG;
            loop++;
        }
    }
}