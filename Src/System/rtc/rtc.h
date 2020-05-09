#ifndef __RTC_H
#define __RTC_H	    

#include "stm32f10x.h"
#include "stm32f10x_rtc.h"

//时间结构体
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;
}_calendar_obj;

extern _calendar_obj calendar;	//日历结构体
void Enter_Standby_Mode(uint16_t Delay_s);
void Enter_Sleep_Mode(uint16_t Delay_s);
u8 RTC_Init(void);        //初始化RTC,返回0,失败;1,成功;

#endif


