#ifndef __USART_H
#define __USART_H

#include <stdio.h>
#include <stdbool.h>
#include "sys.h"
#include "delay.h"

#define USART_REC_LEN           1024    //定义最大接收字节数
#define EN_USART_RX  1

extern uint8_t  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern volatile uint16_t USART1_RX_STA;                //接收状态标记
void USART1_Init(uint32_t bound);

extern uint8_t  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern volatile uint16_t USART3_RX_STA;               //接收状态标记
void USART3_Init(uint32_t bound);

uint8_t SendMessage(USART_TypeDef* usart, uint8_t data[], unsigned int len);

#endif /* __USART_H */
