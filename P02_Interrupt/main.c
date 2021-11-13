#include "stm32f411xe.h"

#define TRACE_ITM_PORT  0
#include "trace.h"

#define HSI_VALUE   16000000U
#define SWO_FREQ    2000000U

// User button (PA0), check schematic
#define GPIO_PORT   GPIOA
#define GPIO_PIN    0

#define EXTI_LINE   GPIO_PIN

// Interrupt trigger flag (atomic access)
static volatile uint32_t itFlag = 0;

void SystemInit()
{
    ITM_Init(0x1 << TRACE_ITM_PORT, SWO_FREQ, HSI_VALUE);
}

void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (0x1U << EXTI_LINE)) {   // Check interrupt occurence
        itFlag = 1;                         // Set the interrupt flag
        EXTI->PR |= (0x1U << EXTI_LINE);    // Clear the pending bit by writing 1
    }
}

int main(void)
{
    ITM_Trace("Program started!\n", 18);

    // Enable clock source for GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;

    GPIOA->MODER &= ~(0x11U << (GPIO_PIN * 2));     // GPIO input (default)
    GPIOA->PUPDR &= ~(0x11U << (GPIO_PIN * 2));     // No pull-down according to schematic
    
    // Enable clock source for SYSCFG
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN_Msk;

    // Ensure PA0 is the source input for EXTI0 (although no need to do so because it is default)
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0_Msk;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;

    EXTI->IMR |= (0x1U << EXTI_LINE);     // Unmask EXTI0
    EXTI->FTSR |= (0x1U << EXTI_LINE);    // Enable falling trigger

    NVIC_EnableIRQ(EXTI0_IRQn);

    while (1) {
        if (itFlag) {
            /* Note: interrupts occured in this block will be ignored */
            const char msg[] = "Button released!\n";
            ITM_Trace(msg, sizeof(msg));
            itFlag = 0;
        }
    }

    return 0;
}