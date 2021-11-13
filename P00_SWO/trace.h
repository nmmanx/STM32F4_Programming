#ifndef __TRACE_H_
#define __TRACE_H_

#include <stdint.h>

#ifndef TRACE_ITM_PORT
#define TRACE_ITM_PORT 0
#endif

void ITM_Init(uint32_t portMask, uint32_t swoFreq, uint32_t refFreq);

void ITM_Write(uint8_t byte, uint8_t port);

void ITM_Trace(const char *msg, uint16_t len);

#endif