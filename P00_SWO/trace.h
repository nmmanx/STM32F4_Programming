#ifndef __TRACE_H_
#define __TRACE_H_

#include <stdint.h>

void ITM_Init(uint32_t portMask, uint32_t swoFreq, uint32_t refFreq);

void ITM_Write(uint8_t byte, uint8_t port);

#endif