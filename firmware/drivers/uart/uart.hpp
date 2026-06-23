#pragma once

#include <cstdint>

namespace Drivers::Uart
{
    void BSP_Uart_Init(void);
    void BSP_Uart_Write_Byte(char c);
}