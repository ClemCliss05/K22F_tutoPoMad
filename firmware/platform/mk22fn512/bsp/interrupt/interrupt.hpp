#pragma once

#include <cstdint>

namespace Interrupt {
// -------------------------------------------------------------------------
// PIT0
// -------------------------------------------------------------------------

// System tick incremented by PITx_IRQHandler().
extern volatile uint64_t pitInterruptCount[4];

// -------------------------------------------------------------------------
// FTM2
// -------------------------------------------------------------------------

// Number of FTM2 counter overflows.
extern volatile uint32_t ftm2OverflowCount;

// Timestamp of the latest FTM2 capture.
extern volatile uint32_t ftm2CaptureTime;

// Logic level of the captured signal.
// false = LOW
// true  = HIGH
extern volatile bool ftm2CaptureLevel;

// Set to true by the ISR when a new capture is available.
extern volatile bool ftm2CapturePending;
} // namespace Interrupt