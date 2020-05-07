#ifndef __WIND_SPEED_SENSOR_H
#define __WIND_SPEED_SENSOR_H

#include "stm32f10x.h"
#include "stdbool.h"

#define ADDRESSCODE 0x01
#define FUNTIONCODE 0x03

void WSS_Init(void);
uint16_t CRC16_MODBUS(uint8_t *data, uint8_t datalen);
bool WSS_CheckMSG(uint8_t *buf);

#endif /* __WIND_SPEED_SENSOR_H */



