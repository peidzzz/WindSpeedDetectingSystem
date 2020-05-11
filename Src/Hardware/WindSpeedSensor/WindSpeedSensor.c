#include "WindSpeedSensor.h"
#include "usart.h"
#include "crc.h"
#include "1602.h"
#include "GSM.h"

bool WSS_RECEDataFLAG;
uint8_t RequestSpeedDataMessage[8] = {0}; // 读取风速数据的消息
uint16_t WindSpeedData = 0;

void WSS_RequestMSG_Init(uint8_t *buf){
  uint8_t offset = 0;
  buf[offset++]  = ADDRESSCODE;
  buf[offset++]  = FUNTIONCODE;
  buf[offset++]  = 0;
  buf[offset++]  = 0;
  buf[offset++]  = 0;
  buf[offset++]  = 0x01;
  buf[offset++]  = 0x84;
  buf[offset++]  = 0x0A;
}

bool WSS_CheckMSG(uint8_t *buf){
  uint8_t CRC16high = 0;
  uint8_t CRC16low  = 0;
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

void PrintWSSBuffer(void){
  uint8_t WSSDataDisBuf[16] = "S: 00.0m/s";
  uint8_t i = 0;
  
  WSS_RECEDataFLAG = false;
  WSS_RequestMSG();
  delay_ms(200);
  while( !WSS_RECEDataFLAG ){  // 等待接收数据
    WSS_RequestMSG();
    delay_ms(1200);
  }
  WSSDataDisBuf[6] = (WindSpeedData % 10) + 48; 
  WSSDataDisBuf[4] = (WindSpeedData % 100) / 10 + 48; 
  WSSDataDisBuf[3] = (WindSpeedData / 100) + 48;
  for(i = 0;i < 16;i++){
    lcd_char_write(i, 1, WSSDataDisBuf[i]);
  }
}

void WSS_Init(void)
{
  /* 初始化风速传感器需要用到的变量及外围硬件 */
  WSS_RequestMSG_Init(RequestSpeedDataMessage);
  USART3_Init(4800); // 初始化
}


