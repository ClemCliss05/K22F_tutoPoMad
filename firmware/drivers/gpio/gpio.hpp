#pragma once

#include <cstdint>

namespace Drivers::Gpio {
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

void LED_Init(void);
void LED_On(void);
void LED_On(uint8_t color);
void LED_Off(void);
void LED_Off(uint8_t color);
void LED_Toggle(void);
void LED_Toggle(uint8_t color);

/*
 * Push Button driver functions
 */

void PBs_Init(void);
bool PB1_GetState(void);
bool PB2_GetState(void);
} // namespace Drivers::Gpio