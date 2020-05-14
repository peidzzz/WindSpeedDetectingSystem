#ifndef __WIND_SPEED_SENSOR_H
#define __WIND_SPEED_SENSOR_H

#include "stm32f10x.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"


#define ADDRESSCODE 0x01  // 风速传感器地址码
#define FUNTIONCODE 0x03  // 风速传感器功能码
extern uint16_t WindSpeedData;

#define STM32FLASH_WSS_GPS_ADDRESS 0x00070000
#define STM32FLASH_COUNT_ADDRESS  (STM32FLASH_WSS_GPS_ADDRESS - 2048)

void WSS_Init(void);
bool WSS_CheckMSG(uint8_t *buf);
void WSS_RequestMSG(void);
void PrintWSSBuffer(void);
void DataCountWrite(uint16_t *count);
uint16_t DataCountRead(void);

#endif /* __WIND_SPEED_SENSOR_H */



