#pragma once

#include <cstdint>

// Global variable and constant used by the drivers
// PIT0
extern volatile uint32_t pit0Ticks;

// FTM2
extern volatile uint32_t ftm2OverflowCount;
extern volatile uint32_t ftm2PressTime;
extern volatile uint32_t ftm2ReleaseTime;

extern volatile bool ftm2ButtonPressed;
extern volatile bool ftm2MeasurementReady;
extern volatile uint32_t ftm2LastEdgeMs;