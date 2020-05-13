#include "GSM.h"
#include "usart.h"

//sim800C���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//����,�ڴ�Ӧ������λ��(str��λ��)
u8* sim800c_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART1_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//��sim800C��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 sim800c_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART1_RX_STA=0;
	printf("%s\r\n",cmd);
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{ 
	   while(--waittime)	//�ȴ�����ʱ
	   {
		   delay_ms(10);
		   if(USART1_RX_STA&0X8000)//���յ��ڴ���Ӧ����
		   {
			   if(sim800c_check_cmd(ack))break;//�õ���Ч���� 
			   USART1_RX_STA=0;
		   } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
} 

//SIM800C�����Ų��� 
void sim800c_sms_send(uint8_t *p1)
{
	u8 phonenum[11] = "15858120627"; 		//���뻺��
	u8 timex=0;
	u8 key=0;
	u8 smssendsta=0;		//���ŷ���״̬,0,�ȴ�����;1,����ʧ��;2,���ͳɹ� 
  
	while(1)
	{
//    sim800c_unigbk_exchange(phonebuf,p,1);				//���绰����ת��Ϊunicode�ַ���
//    sim800c_unigbk_exchange((u8*)sim800c_test_msg,p1,1);//����������ת��Ϊunicode�ַ���.
//    sprintf((char*)p2,"AT+CMGS=\"%s\"",p); 
    if(sim800c_send_cmd("AT+CMGS=\"15858120627\"",">",200)==0)					//���Ͷ�������+�绰����
    { 		 				 													 
      printf("%s",p1);		 						//���Ͷ������ݵ�GSMģ��
      delay_ms(90);                     //������ʱ�������ܷ��Ͷ���						
      if(sim800c_send_cmd((u8*)0X1A,"+CMGS:",1000)==0)smssendsta=2;//���ͽ�����,�ȴ��������(��ȴ�10����,��Ϊ���ų��˵Ļ�,�ȴ�ʱ��᳤һЩ)
    }  
    if(smssendsta==1) printf("Failded");	//��ʾ״̬
    else printf("Succeed");				//��ʾ״̬	
    USART2_RX_STA=0;
  }
} 

/* ��ʼ��SIM800C��Ҫ�õ��ı�������ΧӲ�� */
void GSM_Init(void){
  USART1_Init(38400);
}



