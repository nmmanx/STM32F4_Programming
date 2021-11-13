#include "stm32f411xe.h"

// User LED LD6: PD15
const uint8_t pin = 15;

void SystemInit()
{
    // Enable clock source for the GPIO port
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN_Msk;

    GPIOD->MODER |= (0x1U << (pin * 2));        // GPIO output
    GPIOD->OSPEEDR |= (0x1U << (pin * 2));      // Medium speed
    GPIOD->OTYPER &= ~(0x1U << pin);            // Push-pull (default)
    GPIOD->PUPDR &= ~(0x11U << (pin * 2));      // No pull-up, no pull-down (default)

    // Set default output value to 1
    GPIOD->BSRR |= (0x1U << pin);
}

void SoftDelay(uint32_t count)
{
    while (count > 0) count--;
}

int main()
{
    while (1) {
        if ((GPIOD->ODR & (0x1U << pin)) == 0) {
            GPIOD->BSRR |= (0x1U << pin);           // Set to 1
        } else {
            GPIOD->BSRR |= (0x1U << (pin + 16));    // Set to 0
        }
        SoftDelay(500000);
    }

    return 0;
}