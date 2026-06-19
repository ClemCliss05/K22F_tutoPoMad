#include <stdint.h>

#include "irq_handlers_MK22FN512.hpp"

extern "C"
{
    extern uint32_t _estack;

    extern uint32_t _sidata;
    extern uint32_t _sdata;
    extern uint32_t _edata;

    extern uint32_t _sbss;
    extern uint32_t _ebss;

    extern int main(void);

    extern void SystemInit(void);
    extern void __libc_init_array(void);

    void Reset_Handler(void);
}

using ISR_Handler = void (*)(void);

__attribute__((used))
__attribute__((aligned(256)))
__attribute__((section(".isr_vector")))
const ISR_Handler vector_table[] =
{
    reinterpret_cast<ISR_Handler>(&_estack),

    // Core Level - CM4
    Reset_Handler,                     // The reset handler
    NMI_Handler,                       // The NMI handler
    HardFault_Handler,                 // The hard fault handler
    MemManage_Handler,                 // The MPU fault handler
    BusFault_Handler,                  // The bus fault handler
    UsageFault_Handler,                // The usage fault handler
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    SVC_Handler,                       // SVCall handler
    DebugMon_Handler,                  // Debug monitor handler
    nullptr,                           // Reserved
    PendSV_Handler,                    // The PendSV handler
    SysTick_Handler,                   // The SysTick handler

    // Chip Level - MK22F51212
    DMA0_IRQHandler,            // 16: DMA Channel 0 Transfer Complete
    DMA1_IRQHandler,            // 17: DMA Channel 1 Transfer Complete
    DMA2_IRQHandler,            // 18: DMA Channel 2 Transfer Complete
    DMA3_IRQHandler,            // 19: DMA Channel 3 Transfer Complete
    DMA4_IRQHandler,            // 20: DMA Channel 4 Transfer Complete
    DMA5_IRQHandler,            // 21: DMA Channel 5 Transfer Complete
    DMA6_IRQHandler,            // 22: DMA Channel 6 Transfer Complete
    DMA7_IRQHandler,            // 23: DMA Channel 7 Transfer Complete
    DMA8_IRQHandler,            // 24: DMA Channel 8 Transfer Complete
    DMA9_IRQHandler,            // 25: DMA Channel 9 Transfer Complete
    DMA10_IRQHandler,           // 26: DMA Channel 10 Transfer Complete
    DMA11_IRQHandler,           // 27: DMA Channel 11 Transfer Complete
    DMA12_IRQHandler,           // 28: DMA Channel 12 Transfer Complete
    DMA13_IRQHandler,           // 29: DMA Channel 13 Transfer Complete
    DMA14_IRQHandler,           // 30: DMA Channel 14 Transfer Complete
    DMA15_IRQHandler,           // 31: DMA Channel 15 Transfer Complete
    DMA_Error_IRQHandler,       // 32: DMA Error Interrupt
    MCM_IRQHandler,             // 33: Normal Interrupt
    FTF_IRQHandler,             // 34: FTFA Command complete interrupt
    Read_Collision_IRQHandler,  // 35: Read Collision Interrupt
    LVD_LVW_IRQHandler,         // 36: Low Voltage Detect, Low Voltage Warning
    LLWU_IRQHandler,            // 37: Low Leakage Wakeup Unit
    WDOG_EWM_IRQHandler,        // 38: WDOG Interrupt
    RNG_IRQHandler,             // 39: RNG Interrupt
    I2C0_IRQHandler,            // 40: I2C0 interrupt
    I2C1_IRQHandler,            // 41: I2C1 interrupt
    SPI0_IRQHandler,            // 42: SPI0 Interrupt
    SPI1_IRQHandler,            // 43: SPI1 Interrupt
    I2S0_Tx_IRQHandler,         // 44: I2S0 transmit interrupt
    I2S0_Rx_IRQHandler,         // 45: I2S0 receive interrupt
    LPUART0_IRQHandler,         // 46: LPUART0 status/error interrupt
    UART0_RX_TX_IRQHandler,     // 47: UART0 Receive/Transmit interrupt
    UART0_ERR_IRQHandler,       // 48: UART0 Error interrupt
    UART1_RX_TX_IRQHandler,     // 49: UART1 Receive/Transmit interrupt
    UART1_ERR_IRQHandler,       // 50: UART1 Error interrupt
    UART2_RX_TX_IRQHandler,     // 51: UART2 Receive/Transmit interrupt
    UART2_ERR_IRQHandler,       // 52: UART2 Error interrupt
    Reserved53_IRQHandler,      // 53: Reserved interrupt
    Reserved54_IRQHandler,      // 54: Reserved interrupt
    ADC0_IRQHandler,            // 55: ADC0 interrupt
    CMP0_IRQHandler,            // 56: CMP0 interrupt
    CMP1_IRQHandler,            // 57: CMP1 interrupt
    FTM0_IRQHandler,            // 58: FTM0 fault, overflow and channels interrupt
    FTM1_IRQHandler,            // 59: FTM1 fault, overflow and channels interrupt
    FTM2_IRQHandler,            // 60: FTM2 fault, overflow and channels interrupt
    Reserved61_IRQHandler,      // 61: Reserved interrupt
    RTC_IRQHandler,             // 62: RTC interrupt
    RTC_Seconds_IRQHandler,     // 63: RTC seconds interrupt
    PIT0_IRQHandler,            // 64: PIT timer channel 0 interrupt
    PIT1_IRQHandler,            // 65: PIT timer channel 1 interrupt
    PIT2_IRQHandler,            // 66: PIT timer channel 2 interrupt
    PIT3_IRQHandler,            // 67: PIT timer channel 3 interrupt
    PDB0_IRQHandler,            // 68: PDB0 Interrupt
    USB0_IRQHandler,            // 69: USB0 interrupt
    Reserved70_IRQHandler,      // 70: Reserved interrupt
    Reserved71_IRQHandler,      // 71: Reserved interrupt
    DAC0_IRQHandler,            // 72: DAC0 interrupt
    MCG_IRQHandler,             // 73: MCG Interrupt
    LPTMR0_IRQHandler,          // 74: LPTimer interrupt
    PORTA_IRQHandler,           // 75: Port A interrupt
    PORTB_IRQHandler,           // 76: Port B interrupt
    PORTC_IRQHandler,           // 77: Port C interrupt
    PORTD_IRQHandler,           // 78: Port D interrupt
    PORTE_IRQHandler,           // 79: Port E interrupt
    SWI_IRQHandler,             // 80: Software interrupt
    Reserved81_IRQHandler,      // 81: Reserved interrupt
    Reserved82_IRQHandler,      // 82: Reserved interrupt
    Reserved83_IRQHandler,      // 83: Reserved interrupt
    Reserved84_IRQHandler,      // 84: Reserved interrupt
    Reserved85_IRQHandler,      // 85: Reserved interrupt
    Reserved86_IRQHandler,      // 86: Reserved interrupt
    FTM3_IRQHandler,            // 87: FTM3 fault, overflow and channels interrupt
    DAC1_IRQHandler,            // 88: DAC1 interrupt
    ADC1_IRQHandler,            // 89: ADC1 interrupt

};

extern "C"
void Reset_Handler(void)
{
    SystemInit();

    uint32_t* src = &_sidata;
    uint32_t* dst = &_sdata;

    while (dst < &_edata)
    {
        *dst++ = *src++;
    }

    dst = &_sbss;

    while (dst < &_ebss)
    {
        *dst++ = 0U;
    }

    __libc_init_array();

    (void)main();

    while (1)
    {
        __asm volatile("wfi");
    }
}

extern "C"
void Default_Handler(void)
{
    while (1)
    {
        __asm volatile("wfi");
    }
}