#ifndef __USART_H
#define __USART_H

#include <stdio.h>
#include <stdbool.h>
#include "sys.h"
#include "delay.h"
#include "string.h"

#define USART_REC_LEN           1024    //定义最大接收字节数
#define EN_USART_RX  1

//定义数组长度
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//经度
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//定位信息是否有效
} _SaveData;

extern _SaveData Save_Data;

extern uint8_t  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern volatile uint16_t USART1_RX_STA;                //接收状态标记
void USART1_Init(uint32_t bound);

extern uint8_t  USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern volatile uint16_t USART2_RX_STA;                //接收状态标记
void USART2_Init(uint32_t bound);

extern uint8_t  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern volatile uint16_t USART3_RX_STA;               //接收状态标记
void USART3_Init(uint32_t bound);

uint8_t SendMessage(USART_TypeDef* usart, uint8_t data[], unsigned int len);

#endif /* __USART_H */
