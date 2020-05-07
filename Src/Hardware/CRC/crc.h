#ifndef __CRC_H
#define __CRC_H
#include "stm32f10x.h"
#include "stdbool.h"

uint16_t CRC16_MODBUS(uint8_t *data, uint8_t datalen);

#endif /* __CRC_H */
