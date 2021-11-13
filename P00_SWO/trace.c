#include "stm32f411xe.h"
#include "core_cm4.h"
#include "trace.h"

void ITM_Init(uint32_t portMask, uint32_t swoFreq, uint32_t refFreq)
{
    uint32_t preScaler = (refFreq / swoFreq) - 1;
    
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    
    TPI->SPPR |= 0x2U;
    TPI->ACPR = preScaler;

    ITM->TER |= portMask;
    ITM->TPR |= ITM_TPR_PRIVMASK_Msk;
    ITM->TCR |= ITM_TCR_ITMENA_Msk;
}

void ITM_Write(uint8_t byte, uint8_t port)
{
    if ((ITM->TCR & ITM_TCR_ITMENA_Msk) == 0
            || (ITM->TER && (0x1 << port)) == 0) {
        return;
    }
    
    while (ITM->PORT[port].u8 == 0);
    
    ITM->PORT[port].u8 = byte;
}

void ITM_Trace(const char *msg, uint16_t len)
{
    for (uint16_t i = 0; i < len; ++i) {
        ITM_Write((uint8_t)msg[i], TRACE_ITM_PORT);
    }
}
