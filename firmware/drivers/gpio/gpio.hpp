#pragma once

#include <cstdint>

namespace Drivers::GPIO
{
    /*
    * LED driver functions
    */

    constexpr uint8_t LED_RED = 0x00;
    constexpr uint8_t LED_GREEN = 0x01;
    constexpr uint8_t LED_BLUE = 0x02;
    constexpr uint8_t LED_CYAN = 0x03;
    constexpr uint8_t LED_YELLOW = 0x04;
    constexpr uint8_t LED_MAGENTA = 0x05;
    constexpr uint8_t LED_WHITE = 0x06;

    void	BSP_LED_Init	(void);
    void	BSP_LED_On	    (void);
    void	BSP_LED_On	    (uint8_t color);
    void	BSP_LED_Off	    (void);
    void	BSP_LED_Off	    (uint8_t color);
    void	BSP_LED_Toggle	(void);
    void	BSP_LED_Toggle	(uint8_t color);


    /*
    * Push Button driver functions
    */

    void BSP_PBs_Init(void);
    bool BSP_PB1_GetState(void);
    bool BSP_PB2_GetState(void);
}