#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"

_calendar_obj calendar;//时钟结构体

static void RTC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		        //RTC全局中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	      //先占优先级0位,从优先级4位
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		        //使能该通道中断
  NVIC_Init(&NVIC_InitStructure);		                        //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void Enter_Standby_Mode(uint16_t Delay_s)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  PWR_BackupAccessCmd(ENABLE);
  RTC_SetAlarm(RTC_GetCounter() + Delay_s);
  RTC_WaitForLastTask();

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //使能PWR外设时钟
  PWR_ClearFlag(PWR_FLAG_WU);  //清除Wake—up标志
  PWR_EnterSTANDBYMode();      //进入待机模式（关闭全部外设及内核，仅能被复位、外部中断、RTC闹钟及看门狗唤醒，功耗极低）
}

void Enter_Sleep_Mode(uint16_t Delay_s)
{
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_SetAlarm(RTC_GetCounter() + Delay_s);
	RTC_WaitForLastTask();
	PWR_ClearFlag(PWR_FLAG_WU);   //清除Wake—up标志	
  __WFI();                      //进入睡眠模式（仅关闭内核，外设寄存器保持不变，现象是不执行代码，能被任何中断唤醒）
}

u8 RTC_Init(void)
{
  //检查是不是第一次配置时钟
  u8 temp = 0;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	 //使能PWR和BKP外设时钟
  PWR_BackupAccessCmd(ENABLE);	                           //使能后备寄存器访问
  if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050) {	       //从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎

    BKP_DeInit();	                                         //复位备份区域
    RCC_LSEConfig(RCC_LSE_ON);	                           //设置外部低速晶振(LSE),使用外设低速晶振
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {	 //检查指定的RCC标志位设置与否,等待低速晶振就绪
      temp++;
      delay_ms(10);
    }
    if (temp >= 250)return 1;                 //初始化时钟失败,晶振有问题则返回
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟
    RCC_RTCCLKCmd(ENABLE);	                  //使能RTC时钟
    RTC_WaitForLastTask();                  	//等待最近一次对RTC寄存器的写操作完成
    RTC_WaitForSynchro();		                  //等待RTC寄存器同步
    RTC_ITConfig(RTC_IT_ALR, ENABLE);		      //使能RTC闹钟中断 不使能秒中断
    RTC_WaitForLastTask();	                  //等待最近一次对RTC寄存器的写操作完成
    RTC_EnterConfigMode();                    // 允许配置
    RTC_SetPrescaler(32767);                  //设置RTC预分频的值
    RTC_WaitForLastTask();	                  //等待最近一次对RTC寄存器的写操作完成
    RTC_ExitConfigMode();                     //退出配置模式
    BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//向指定的后备寄存器中写入用户程序数据
  } else { 
    //系统继续计时
    RTC_WaitForSynchro();	            //等待最近一次对RTC寄存器的写操作完成
    RTC_ITConfig(RTC_IT_ALR, ENABLE);	//使能RTC闹钟中断 不使能秒中断
    RTC_WaitForLastTask();	          //等待最近一次对RTC寄存器的写操作完成
  }
  RTC_NVIC_Config();                  //RCT中断分组设置
  return 0; 

}

//RTC时钟中断
//每秒触发一次
//extern u16 tcnt;
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {       //秒钟中断（没有用到所以没有使能）
  }
  if (RTC_GetITStatus(RTC_IT_ALR) != RESET) {       //闹钟中断
    RTC_ClearITPendingBit(RTC_IT_ALR);		          //清闹钟中断
  }
  RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);		//清秒中断标志位 清溢出标志位
  RTC_WaitForLastTask();
}




