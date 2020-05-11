#ifndef __WIND_SPEED_SENSOR_H
#define __WIND_SPEED_SENSOR_H

#include "stm32f10x.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

#define ADDRESSCODE 0x01
#define FUNTIONCODE 0x03

extern uint16_t WindSpeedData;

void WSS_Init(void);
bool WSS_CheckMSG(uint8_t *buf);
void WSS_RequestMSG(void);
void PrintWSSBuffer(void);

#endif /* __WIND_SPEED_SENSOR_H */



