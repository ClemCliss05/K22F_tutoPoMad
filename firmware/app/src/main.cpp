#include "clock.hpp"
#include "gpio.hpp"

int main()
{
    Drivers::GPIO::BSP_LED_Init();
    Drivers::GPIO::BSP_PBs_Init();
    Drivers::GPIO::BSP_LED_On();
    for (volatile uint32_t i = 0; i < 5000000; i++);
    Drivers::GPIO::BSP_LED_Off();

    while (1)
    {
        if(Drivers::GPIO::BSP_PB1_GetState())
        {
            Drivers::GPIO::BSP_LED_On(Drivers::GPIO::LED_GREEN);
        }
        else
        {
            Drivers::GPIO::BSP_LED_Off(Drivers::GPIO::LED_GREEN);
        }

        if(Drivers::GPIO::BSP_PB2_GetState())
        {
            Drivers::GPIO::BSP_LED_On(Drivers::GPIO::LED_BLUE);
        }
        else
        {
            Drivers::GPIO::BSP_LED_Off(Drivers::GPIO::LED_BLUE);
        }
    }
}