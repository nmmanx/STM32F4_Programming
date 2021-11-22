#include "stm32f411xe.h"

#define TRACE_ITM_PORT  0
#include "trace.h"

#define HSI_VALUE   16000000U
#define SWO_FREQ    2000000U

void SystemInit()
{
    ITM_Init(0x1 << TRACE_ITM_PORT, SWO_FREQ, HSI_VALUE);
}

void PinConfigSPI1()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN_Msk;

    // MISO: PB4
    GPIOB->MODER |= (0x2U << (4 * 2));      // Alternate function mode
    GPIOB->AFR[0] |= (0x5U << (4 * 4));     // AF05

    // MOSI: PB5
    GPIOB->MODER |= (0x2U << (5 * 2));      // Alternate function mode
    GPIOB->AFR[0] |= (0x5U << (5 * 4));     // AF05

    // CK: PA5
    GPIOA->MODER |= (0x2U << (5 * 2));      // Alternate function mode
    GPIOA->AFR[0] |= (0x5U << (5 * 4));     // AF05

    // NSS: PA5
    GPIOA->MODER |= (0x2U << (15 * 2));             // Alternate function mode
    GPIOA->AFR[1] |= (0x5U << ((15 % 8) * 4));      // AF05
    GPIOA->ODR |= (0x1U << 15);                     // Open-drain
    GPIOA->PUPDR |= (0x1U << (15 * 2));             // Pull-up
}

void PinConfigSPI2()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN_Msk;

    // MISO: PB14
    GPIOB->MODER |= (0x2U << (14 * 2));         // Alternate function mode
    GPIOB->AFR[1] |= (0x5U << ((14 % 8) * 4));  // AF05

    // MOSI: PB15
    GPIOB->MODER |= (0x2U << (15 * 2));         // Alternate function mode
    GPIOB->AFR[1] |= (0x5U << ((15 % 8) * 4));  // AF05

    // CK: PB13
    GPIOB->MODER |= (0x2U << (13 * 2));         // Alternate function mode
    GPIOB->AFR[1] |= (0x5U << ((13 % 8) * 4));  // AF05

    // NSS: PB12
    GPIOB->MODER |= (0x2U << (12 * 2));         // Alternate function mode
    GPIOB->AFR[1] |= (0x5U << ((12 % 8) * 4));  // AF05
}

void ConfigSPI1()
{
    PinConfigSPI1();
    
    // Enable SPI1 clock source
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN_Msk;

    SPI1->CR1 |= 0x1U << SPI_CR1_BR_Pos;        // Baudrate fclk/4
    SPI1->CR1 |= 0x1U << SPI_CR1_DFF_Pos;       // 16 bit data frame
    SPI1->CR1 |= 0x1U << SPI_CR1_LSBFIRST_Pos;  // LSB transmitted first
    SPI1->CR1 |= 0x1U << SPI_CR1_MSTR_Pos;      // Master configuration
    SPI1->CR1 |= 0x1U << SPI_CR1_SPE_Pos;       // Enable SPI

    // Default: SSM=0 (NSS hardware mode), 
    //          CPOL=0, CPHA=0
}

void ConfigSPI2()
{
    PinConfigSPI2();

    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN_Msk;

    SPI1->CR1 |= 0x1U << SPI_CR1_DFF_Pos;       // 16 bit data frame
    SPI1->CR1 |= 0x1U << SPI_CR1_LSBFIRST_Pos;  // LSB transmitted first
    SPI1->CR1 |= 0x1U << SPI_CR1_SPE_Pos;       // Enable SPI

    // Default: MSTR=0 (slave), SSM=0 (NSS hardware mode),
    //          CPOL=0, CPHA=0
}

int main()
{

    return 0;
}