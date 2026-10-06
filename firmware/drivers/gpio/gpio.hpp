#pragma once

#include <cstdint>

namespace Drivers {
/**
 * Configures and controls K22F GPIO pins.
 */
class Gpio {
  public:
    enum class LedColor : uint8_t { Red, Green, Blue, Cyan, Yellow, Magenta, White };

    /*
     * LED driver functions
     * Pin PTA1 -> LEDRGB_RED
     * Pin PTA2 -> LEDRGB_GREEN
     * Pin PTD5 -> LEDRGB_BLUE
     */
    void LED_Init(void);
    void LED_On(void);
    void LED_On(LedColor color);
    void LED_Off(void);
    void LED_Off(LedColor color);
    void LED_Toggle(void);
    void LED_Toggle(LedColor color);

    /*
     * Push Button driver functions
     * PTB17 -> PB1
     * PTC1  -> PB2
     */
    void PBs_Init(void);
    bool PB1_GetState(void);
    bool PB2_GetState(void);

  private:
};
} // namespace Drivers