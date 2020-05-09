#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"

_calendar_obj calendar;//ʱ�ӽṹ��

static void RTC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		        //RTCȫ���ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1λ,�����ȼ�3λ
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	      //��ռ���ȼ�0λ,�����ȼ�4λ
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		        //ʹ�ܸ�ͨ���ж�
  NVIC_Init(&NVIC_InitStructure);		                        //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void Enter_Standby_Mode(uint16_t Delay_s)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  PWR_BackupAccessCmd(ENABLE);
  RTC_SetAlarm(RTC_GetCounter() + Delay_s);
  RTC_WaitForLastTask();

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //ʹ��PWR����ʱ��
  PWR_ClearFlag(PWR_FLAG_WU);  //���Wake��up��־
  PWR_EnterSTANDBYMode();      //�������ģʽ���ر�ȫ�����輰�ںˣ����ܱ���λ���ⲿ�жϡ�RTC���Ӽ����Ź����ѣ����ļ��ͣ�
}

void Enter_Sleep_Mode(uint16_t Delay_s)
{
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_SetAlarm(RTC_GetCounter() + Delay_s);
	RTC_WaitForLastTask();
	PWR_ClearFlag(PWR_FLAG_WU);   //���Wake��up��־	
  __WFI();                      //����˯��ģʽ�����ر��ںˣ�����Ĵ������ֲ��䣬�����ǲ�ִ�д��룬�ܱ��κ��жϻ��ѣ�
}

u8 RTC_Init(void)
{
  //����ǲ��ǵ�һ������ʱ��
  u8 temp = 0;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	 //ʹ��PWR��BKP����ʱ��
  PWR_BackupAccessCmd(ENABLE);	                           //ʹ�ܺ󱸼Ĵ�������
  if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050) {	       //��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����

    BKP_DeInit();	                                         //��λ��������
    RCC_LSEConfig(RCC_LSE_ON);	                           //�����ⲿ���پ���(LSE),ʹ��������پ���
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {	 //���ָ����RCC��־λ�������,�ȴ����پ������
      temp++;
      delay_ms(10);
    }
    if (temp >= 250)return 1;                 //��ʼ��ʱ��ʧ��,�����������򷵻�
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��
    RCC_RTCCLKCmd(ENABLE);	                  //ʹ��RTCʱ��
    RTC_WaitForLastTask();                  	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_WaitForSynchro();		                  //�ȴ�RTC�Ĵ���ͬ��
    RTC_ITConfig(RTC_IT_ALR, ENABLE);		      //ʹ��RTC�����ж� ��ʹ�����ж�
    RTC_WaitForLastTask();	                  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_EnterConfigMode();                    // ��������
    RTC_SetPrescaler(32767);                  //����RTCԤ��Ƶ��ֵ
    RTC_WaitForLastTask();	                  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_ExitConfigMode();                     //�˳�����ģʽ
    BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//��ָ���ĺ󱸼Ĵ�����д���û���������
  } else { 
    //ϵͳ������ʱ
    RTC_WaitForSynchro();	            //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_ITConfig(RTC_IT_ALR, ENABLE);	//ʹ��RTC�����ж� ��ʹ�����ж�
    RTC_WaitForLastTask();	          //�ȴ����һ�ζ�RTC�Ĵ�����д�������
  }
  RTC_NVIC_Config();                  //RCT�жϷ�������
  return 0; 

}

//RTCʱ���ж�
//ÿ�봥��һ��
//extern u16 tcnt;
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {       //�����жϣ�û���õ�����û��ʹ�ܣ�
  }
  if (RTC_GetITStatus(RTC_IT_ALR) != RESET) {       //�����ж�
    RTC_ClearITPendingBit(RTC_IT_ALR);		          //�������ж�
  }
  RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);		//�����жϱ�־λ �������־λ
  RTC_WaitForLastTask();
}




