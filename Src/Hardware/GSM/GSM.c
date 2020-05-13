#include "GSM.h"
#include "usart.h"

//sim800C发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//其他,期待应答结果的位置(str的位置)
u8* sim800c_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART1_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//向sim800C发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim800c_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART1_RX_STA=0;
	printf("%s\r\n",cmd);
	if(ack&&waittime)		//需要等待应答
	{ 
	   while(--waittime)	//等待倒计时
	   {
		   delay_ms(10);
		   if(USART1_RX_STA&0X8000)//接收到期待的应答结果
		   {
			   if(sim800c_check_cmd(ack))break;//得到有效数据 
			   USART1_RX_STA=0;
		   } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
} 

//SIM800C发短信测试 
void sim800c_sms_send(uint8_t *p1)
{
	u8 phonenum[11] = "15858120627"; 		//号码缓存
	u8 timex=0;
	u8 key=0;
	u8 smssendsta=0;		//短信发送状态,0,等待发送;1,发送失败;2,发送成功 
  
	while(1)
	{
//    sim800c_unigbk_exchange(phonebuf,p,1);				//将电话号码转换为unicode字符串
//    sim800c_unigbk_exchange((u8*)sim800c_test_msg,p1,1);//将短信内容转换为unicode字符串.
//    sprintf((char*)p2,"AT+CMGS=\"%s\"",p); 
    if(sim800c_send_cmd("AT+CMGS=\"15858120627\"",">",200)==0)					//发送短信命令+电话号码
    { 		 				 													 
      printf("%s",p1);		 						//发送短信内容到GSM模块
      delay_ms(90);                     //必须延时，否则不能发送短信						
      if(sim800c_send_cmd((u8*)0X1A,"+CMGS:",1000)==0)smssendsta=2;//发送结束符,等待发送完成(最长等待10秒钟,因为短信长了的话,等待时间会长一些)
    }  
    if(smssendsta==1) printf("Failded");	//显示状态
    else printf("Succeed");				//显示状态	
    USART2_RX_STA=0;
  }
} 

/* 初始化SIM800C需要用到的变量及外围硬件 */
void GSM_Init(void){
  USART1_Init(38400);
}



