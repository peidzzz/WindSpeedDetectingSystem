#include "stm32f10x.h"
#include "stmflash.h"
#include "delay.h"
#include "usart.h"
#include "WindSpeedSensor.h"
#include "GSM.h"
#include "rtc.h"
bool WSS_DATAFLAG;

int main(void){
  
  delay_init();
  WSS_Init();
  GSM_Init();
  
  while(1){
    /* 向风速传感器请求数据 */
    WSS_DATAFLAG = false;
    WSS_RequestMSG();
    delay_ms(200);
    while( !WSS_DATAFLAG ){  // 等待接收数据
      WSS_RequestMSG();
      delay_ms(200);
    }
    
    /* 使用FLASH储存风速数据 满足条件后发送至上位机 */
    
//    WindDataStorage();
//    if(  ){ // 当存储够6个之后 将六条数据发送至上位机
//      
//    }
    
    /* 显示当前时间 */
    
    /* 设置RTC闹钟 进入睡眠模式 以600s（10分钟）为周期唤醒 */
    delay_ms(1000);          // 延时1s，避免不必要的麻烦
    Enter_Sleep_Mode(599); // 进入睡眠模式，599s后唤醒 
    
  }
}
