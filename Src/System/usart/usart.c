#include "sys.h"
#include "usart.h"	  
#include "crc.h"
#include "WindSpeedSensor.h"


u16 point1 = 0;

_SaveData Save_Data;
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
  USART2->DR = (u8) ch;      
	return ch;
}
#endif 

extern bool WSS_RECEDataFLAG;

#if EN_USART_RX   //如果使能了接收

void USART1_Init(uint32_t bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); //使能USART1，GPIOA时钟

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);           //初始化GPIOA.9

    //USART1_RX   GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;           //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                //初始化GPIOA.10

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;      //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                                //根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate            = bound;                          //串口波特率
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            //字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               //一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //收发模式

    USART_Init(USART1, &USART_InitStructure);      //初始化串口1
    USART_Cmd(USART1, ENABLE);                     //使能串口1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启串口接受中断
    USART_ClearFlag(USART1, USART_FLAG_TC);        //清除 USARTx 的待处理标志位
}

void USART2_Init(uint32_t bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    //USART_ClockInitTypeDef USART_ClockInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2

    //USART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);           //初始化GPIOA.2
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);

    //USART2_RX   GPIOA.3初始化
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;            //PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                //初始化GPIOA.3

    //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;      //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                                //根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate            = bound;                          //串口波特率
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            //字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               //一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //收发模式

    USART_Init(USART2, &USART_InitStructure);      //初始化串口2
    USART_Cmd(USART2, ENABLE);                     //使能串口2
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启串口接受中断
    USART_ClearFlag(USART2, USART_FLAG_TC);        //清除 USARTx 的待处理标志位

}

void USART3_Init(uint32_t bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART3

    //USART3_TX   GPIOB.10
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10; //PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);           //初始化GPIOB.10

    //USART3_RX   GPIOB.11初始化
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;           //PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);                //初始化GPIOB.11

    //Usart3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;      //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                                //根据指定的参数初始化VIC寄存器

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate            = bound;                          //串口波特率
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            //字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               //一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //收发模式

    USART_Init(USART3, &USART_InitStructure);      //初始化串口3
    USART_Cmd(USART3, ENABLE);                     //使能串口3
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启串口接受中断
    USART_ClearFlag(USART3, USART_FLAG_TC);        //清除 USARTx 的待处理标志位
}

//串口1中断服务程序
volatile uint16_t USART1_RX_STA = 0;            //接收状态标记
uint8_t           USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
uint16_t          Usart1_Rx = 0;

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
  {
    Res =USART_ReceiveData(USART1);	//读取接收到的数据
  
    if((USART1_RX_STA&0x8000)==0)//接收未完成
    {
      if(USART1_RX_STA&0x4000)//接收到了0x0d
      {
        if(Res!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
        else USART1_RX_STA|=0x8000;	//接收完成了 
      }
      else //还没收到0X0D
      {	
        if(Res==0x0d)USART1_RX_STA|=0x4000;
        else
        {
          USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
          USART1_RX_STA++;
          if(USART1_RX_STA>(USART_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
        }		 
      }
    }   		 
  } 
} 

//串口2中断服务程序
volatile uint16_t USART2_RX_STA = 0;            //接收状态标记
uint8_t           USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
uint16_t          Usart2_Rx = 0;

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
    if(Res == '$')
    {
      point1 = 0;	
    }
    USART2_RX_BUF[point1++] = Res;

    if(USART2_RX_BUF[0] == '$' && USART2_RX_BUF[4] == 'M' && USART2_RX_BUF[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
    {
      if(Res == '\n')									   
      {
        memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
        memcpy(Save_Data.GPS_Buffer, USART2_RX_BUF, point1); 	//保存数据
        Save_Data.isGetData = true;
        point1 = 0;
        memset(USART2_RX_BUF, 0, USART_REC_LEN);      //清空				
      }	
          
    }
    if(point1 >= USART_REC_LEN)
    {
      point1 = USART_REC_LEN;
    }
  }
}

u8 Hand(char *a)                   // 串口命令识别函数
{ 
    if(strstr((const char *)USART2_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // 串口缓存清理
{
	memset(USART2_RX_BUF, 0, USART_REC_LEN);      //清空
  point1 = 0;                    
}

void clrStruct(void)
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
}

//串口3中断服务程序
volatile uint16_t USART3_RX_STA = 0;
uint8_t           USART3_RX_BUF[USART_REC_LEN];
uint16_t          Usart3_Rx = 0;

void USART3_IRQHandler(void) //串口3中断服务程序
{
  if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET) //中断产生 
  {
    USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清除中断标志  
    USART3_RX_BUF[Usart3_Rx] = USART_ReceiveData(USART3);     //接收串口1数据到buff缓冲区
    Usart3_Rx++;  
    if(Usart3_Rx == 7)    //如果接收到尾标识是换行符（或者等于最大接受数就清空重新接收）
    {
      if(WSS_CheckMSG(USART3_RX_BUF))                      //检测到头标识是我们需要的 
      {
        WSS_RECEDataFLAG = true;
        WindSpeedData = (uint16_t)((((uint16_t)USART3_RX_BUF[3]) << 4) | (uint16_t)USART3_RX_BUF[4]);
      } else {
        Usart3_Rx=0;                                   //不是我们需要的数据或者达到最大接收数则开始重新接收
      }
    }
  }
}

#endif

//通过串口发送数据
uint8_t SendMessage(USART_TypeDef* usart, uint8_t data[], uint32_t len) {  
  uint32_t j;

  assert_param(len > 0);
  assert_param(usart != NULL);

  for (j = 0; j < len; j++) {
    /* 发送一个字节数据到USART */
    USART_SendData(usart, data[j]);
    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(usart, USART_FLAG_TXE) == RESET) {
    }
  }

  return 0;
}

