#include "stm32f10x.h"
#include "stmflash.h"
#include "delay.h"
#include "usart.h"
#include "WindSpeedSensor.h"
#include "GSM.h"

extern uint8_t RequestSpeedDataMessage[8];

int main(void){
  
  delay_init();
  WSS_Init();
  GSM_Init();
  SendMessage(USART3, RequestSpeedDataMessage, 8);
  while(1){
    delay_ms(500);
  }
}
