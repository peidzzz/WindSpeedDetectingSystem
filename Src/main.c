#include "stm32f10x.h"
#include "stmflash.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"

#include "WindSpeedSensor.h"
#include "GSM.h"
#include "1602.h"
#include "gps.h"

uint8_t StoreBuf[20] = {0}; // 一组数据长度为20字节 风速3字节 时间6字节 定位11字节
uint8_t SendBuf[120] = {0};

int main(void){
  
  uint8_t i;
  uint16_t j;
  uint16_t count = 0;
  uint16_t Buf[10] = {0};
  
  delay_init(); // 初始化延时函数
  WSS_Init();   // 初始化风速传感器
  GSM_Init();   // 初始化GSM模块
  GPS_Init();   // 初始化GPS模块
  LCD_Init();   // 初始化LCD1602  
  
  while(1){
    /* 获取当前风速数据,使用lcd1602显示 */
    PrintWSSBuffer();
    
    /* 获取经纬度及UTC时间信息，使用lcd1602显示经纬度 */
    while(ParseGPSBuffer() == 1);
    PrintGPSBuffer();
    
    /* 储存经纬度、时间及风速数据 */
    // 读取当前存储序号 如果满6个则上传
    count = DataCountRead();
    if(count > 6){
      count = 0;
      DataCountWrite(&count);
    }
    if(count <= 5) {
      for (i = 0;i < 20;i += 2 ) {
        Buf[i / 2] = (((uint16_t)StoreBuf[i]) << 8) + (uint16_t)StoreBuf[i + 1];
      }
      STMFLASH_Write(STM32FLASH_WSS_GPS_ADDRESS + count * 20 , Buf, 10);
      count++;
      if(count == 6){
        count = 0;
        for (i = 0;i < 120;i+=2){
          j = STMFLASH_ReadHalfWord(STM32FLASH_WSS_GPS_ADDRESS + i);
          SendBuf[i] = (uint8_t)(j >> 8);
          SendBuf[i + 1] = (uint8_t)(j & 0x00ff);
        }
        sim800c_sms_send(SendBuf);  // 通过GSM模块上传数据
      }
      DataCountWrite(&count);
    }
    
    /* 设置RTC闹钟 进入睡眠模式 以600s（10分钟）为周期唤醒 */
    delay_ms(1000);        // 延时1s，避免不必要的麻烦
    Enter_Sleep_Mode(599); // 进入睡眠模式，599s后唤醒 
    
  }
}
