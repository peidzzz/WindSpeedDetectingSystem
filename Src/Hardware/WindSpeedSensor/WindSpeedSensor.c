#include "WindSpeedSensor.h"
#include "usart.h"
#include "crc.h"

uint8_t RequestSpeedDataMessage[8] = {0}; // 读取风速数据

void WSS_RequestMSG_Init(uint8_t *buf){
  uint8_t offset = 0;
  buf[offset++] = ADDRESSCODE;
  buf[offset++] = FUNTIONCODE;
  buf[offset++] = 0;
  buf[offset++] = 0;
  buf[offset++] = 0;
  buf[offset++] = 0x01;
  buf[offset++] = 0x84;
  buf[offset++] = 0x0A;
}

bool WSS_CheckMSG(uint8_t *buf){
  uint8_t CRC16high = 0;
  uint8_t CRC16low = 0;
  if (buf[0] == ADDRESSCODE && buf[1] == FUNTIONCODE) {
    CRC16high = (uint8_t)(CRC16_MODBUS(buf, 5) & 0xff00) >> 8;
    CRC16low  = (uint8_t)(CRC16_MODBUS(buf, 5) & 0x00ff);
    if (CRC16high == buf[6] && CRC16low == buf[5]) {
      return true;
    }
  }
  return false;
}

void WSS_RequestMSG(void){
  SendMessage(USART3, RequestSpeedDataMessage, 8);
}

void WSS_Init(void)
{
  /* 初始化风速传感器需要用到的变量及外围硬件 */
  WSS_RequestMSG_Init(RequestSpeedDataMessage);
  USART3_Init(4800); // 初始化
}


