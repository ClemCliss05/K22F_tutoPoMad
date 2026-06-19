#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void Default_Handler(void);

// Core Level - CM4

void NMI_Handler()               __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler()         __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler()         __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler()          __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler()        __attribute__((weak, alias("Default_Handler")));
void SVC_Handler()               __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler()          __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler()            __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler()           __attribute__((weak, alias("Default_Handler")));

// Chip Level - MK22F51212

void DMA0_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DMA1_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DMA2_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DMA3_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DMA4_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DMA5_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DMA6_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DMA7_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DMA8_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DMA9_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DMA10_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void DMA11_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void DMA12_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void DMA13_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void DMA14_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void DMA15_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void DMA_Error_IRQHandler()      __attribute__((weak, alias("Default_Handler")));
void MCM_IRQHandler()            __attribute__((weak, alias("Default_Handler")));
void FTF_IRQHandler()            __attribute__((weak, alias("Default_Handler")));
void Read_Collision_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void LVD_LVW_IRQHandler()        __attribute__((weak, alias("Default_Handler")));
void LLWU_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void WDOG_EWM_IRQHandler()       __attribute__((weak, alias("Default_Handler")));
void RNG_IRQHandler()            __attribute__((weak, alias("Default_Handler")));
void I2C0_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void I2C1_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void SPI0_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void I2S0_Tx_IRQHandler()        __attribute__((weak, alias("Default_Handler")));
void I2S0_Rx_IRQHandler()        __attribute__((weak, alias("Default_Handler")));
void LPUART0_IRQHandler()        __attribute__((weak, alias("Default_Handler")));
void UART0_RX_TX_IRQHandler()    __attribute__((weak, alias("Default_Handler")));
void UART0_ERR_IRQHandler()      __attribute__((weak, alias("Default_Handler")));
void UART1_RX_TX_IRQHandler()    __attribute__((weak, alias("Default_Handler")));
void UART1_ERR_IRQHandler()      __attribute__((weak, alias("Default_Handler")));
void UART2_RX_TX_IRQHandler()    __attribute__((weak, alias("Default_Handler")));
void UART2_ERR_IRQHandler()      __attribute__((weak, alias("Default_Handler")));
void Reserved53_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void Reserved54_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void ADC0_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void CMP0_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void CMP1_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void FTM0_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void FTM1_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void FTM2_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void Reserved61_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler()            __attribute__((weak, alias("Default_Handler")));
void RTC_Seconds_IRQHandler()    __attribute__((weak, alias("Default_Handler")));
void PIT0_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void PIT1_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void PIT2_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void PIT3_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void PDB0_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void USB0_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void Reserved70_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void Reserved71_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void DAC0_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void MCG_IRQHandler()            __attribute__((weak, alias("Default_Handler")));
void LPTMR0_IRQHandler()         __attribute__((weak, alias("Default_Handler")));
void PORTA_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void PORTB_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void PORTC_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void PORTD_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void PORTE_IRQHandler()          __attribute__((weak, alias("Default_Handler")));
void SWI_IRQHandler()            __attribute__((weak, alias("Default_Handler")));
void Reserved81_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void Reserved82_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void Reserved83_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void Reserved84_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void Reserved85_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void Reserved86_IRQHandler()     __attribute__((weak, alias("Default_Handler")));
void FTM3_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void DAC1_IRQHandler()           __attribute__((weak, alias("Default_Handler")));
void ADC1_IRQHandler()           __attribute__((weak, alias("Default_Handler")));

#ifdef __cplusplus
}
#endif