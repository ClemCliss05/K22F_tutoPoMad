#pragma once

#include <cstdint>

namespace Drivers {
class Gpio {
    public:
        /*
        * LED driver functions
        */

        enum class LedColor : uint8_t
        {
            Red,
            Green,
            Blue,
            Cyan,
            Yellow,
            Magenta,
            White
        };

        void LED_Init(void);
        void LED_On(void);
        void LED_On(LedColor color);
        void LED_Off(void);
        void LED_Off(LedColor color);
        void LED_Toggle(void);
        void LED_Toggle(LedColor color);

        /*
        * Push Button driver functions
        */

        void PBs_Init(void);
        bool PB1_GetState(void);
        bool PB2_GetState(void);
    private:
};
} // namespace Drivers