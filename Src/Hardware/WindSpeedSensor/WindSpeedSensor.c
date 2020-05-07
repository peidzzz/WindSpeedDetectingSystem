#include "WindSpeedSensor.h"
#include "usart.h"

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

void InvertUint8(uint8_t *dBuf,uint8_t *srcBuf)
{
	uint8_t i;
	uint8_t tmp[4]={0};
 
	for(i=0;i< 8;i++)
	{
		if(srcBuf[0]& (1 << i))
		tmp[0]|=1<<(7-i);
	}
	dBuf[0] = tmp[0];
	
}
void InvertUint16(uint16_t *dBuf,uint16_t *srcBuf)
{
	uint8_t i;
	uint16_t tmp[4]={0};
 
	for(i=0;i< 16;i++)
	{
		if(srcBuf[0]& (1 << i))
		tmp[0]|=1<<(15 - i);
	}
	dBuf[0] = tmp[0];
}

uint16_t CRC16_MODBUS(uint8_t *data, uint8_t datalen){
	uint16_t wCRCin = 0xFFFF;
	uint16_t wCPoly = 0x8005;
	uint8_t wChar = 0;
	
	while (datalen--) 	
	{
		wChar = *(data++);
		InvertUint8(&wChar,&wChar);
		wCRCin ^= (wChar << 8);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin,&wCRCin);
	return (wCRCin);
}

void WSS_Init(void)
{
  /* 初始化风速传感器需要用到的变量及外围硬件 */
  WSS_RequestMSG_Init(RequestSpeedDataMessage);
  USART3_Init(4800); // 初始化
}


