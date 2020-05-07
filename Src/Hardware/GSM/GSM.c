#include "GSM.h"
#include "usart.h"

/* 初始化SIM800C需要用到的变量及外围硬件 */
void GSM_Init(void){
  USART1_Init(38400);
}



