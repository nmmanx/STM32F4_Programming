#include "trace.h"

#define SYSCLK  16000000    // 16 MHz HSI clock source
#define SWOFREQ 2000000     // 2000 kHz SWO frequency

#define STIMULUS_PORT 0

void SystemInit()
{
    ITM_Init(0x1U << STIMULUS_PORT, SWOFREQ, SYSCLK);
}

int main(void)
{
    const char *msg = "Hello world!\n";

    for(int i = 0; msg[i] != '\0'; i++) {
        ITM_Write(msg[i], STIMULUS_PORT);
    }

    return 0;
}