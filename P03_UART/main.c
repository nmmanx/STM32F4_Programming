#include "stm32f411xe.h"
#include "core_cm4.h"

#define TRACE_ITM_PORT  0
#include "trace.h"

#define HSI_VALUE   16000000U
#define SWO_FREQ    2000000U

#define APB2_CLK    HSI_VALUE

void SystemInit()
{
    ITM_Init(0x1 << TRACE_ITM_PORT, SWO_FREQ, HSI_VALUE);
}

void PinConfig()
{
    const int tx = 6;    // PB6 (USART1_TX)
    const int rx = 7;    // PB7 (USART1_RX)

    // Enable GPIOB clock source
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    GPIOB->MODER |= 0x2U << (tx * 2);    // Alternate function mode
    GPIOB->MODER |= 0x2U << (rx * 2);    // Alternate function mode

    GPIOB->OSPEEDR |= 0x3U << (tx * 2);    // High speed output
    GPIOB->OSPEEDR |= 0x3U << (rx * 2);    // High speed output

    // default GPIO: push-pull, no pupd

    GPIOB->AFR[0] |= 0x7U << (tx * 4);  // Select alternate function AF07 for PB6
    GPIOB->AFR[0] |= 0x7U << (rx * 4);  // Select alternate function AF07 for PB7
}

void UartConfig(USART_TypeDef *usart, uint32_t busClk, uint32_t baudrate)
{
    usart->CR1 |= USART_CR1_OVER8_Msk;  // Oversampling by 8 bit
    usart->CR1 |= USART_CR1_UE_Msk;     // Enable UART
    // CR1 default: 8 data bits, no parity

    usart->CR1 |= USART_CR1_TE_Msk;    // Enable transmitter
    usart->CR1 |= USART_CR1_RE_Msk;    // Enable receiver

    // CR2 default: 1 stop bit

    uint8_t over8 = 0x1U & (usart->CR1 >> USART_CR1_OVER8_Pos);
    float usartDiv = ((float)busClk / baudrate) / (8 * (2 - over8));

    uint16_t mantissa = (uint16_t)usartDiv;
    uint8_t fraction = (uint8_t)(8.0f * (usartDiv - mantissa) + 0.5f);

    usart->BRR |= (fraction & 0x7U) << USART_BRR_DIV_Fraction_Pos;
    usart->BRR |= mantissa << USART_BRR_DIV_Mantissa_Pos;
}

int main()
{
    USART_TypeDef *usart = USART1;
    uint8_t buffer[50];
    uint16_t size = 0;

    PinConfig();

    // Enable USART1 clock source
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // Configure USART for 8-N-1, 9600 bps
    UartConfig(usart, APB2_CLK, 9600);

    while (1) {
        if (usart->SR & USART_SR_RXNE_Msk) {  // Data ready for read;
            buffer[size] = (usart->DR & 0xFFU);
            ITM_Trace(&buffer[size], 1);
            size++;
            usart->SR &= ~USART_SR_RXNE_Msk;     // Clear RXNE
        }

        if ((size > 0 && buffer[size - 1] == '\n') || size == sizeof(buffer)) {
            for (uint8_t i = 0; i < size; ++i) {
                // Wait until data is transfered to shift register
                while (!(usart->SR & USART_SR_TXE_Msk)) { 
                    __NOP();
                }
                usart->DR = (uint8_t)buffer[i]; // Write data into data register
            }

            // Wait until all data is transmited
            while (!(usart->SR & USART_SR_TC_Msk)) {
                __NOP();
            }

            usart->SR &= ~USART_SR_TC_Msk; // Clear TC bit
            size = 0; // Reset buffer
        }
    }

    return 0;
}