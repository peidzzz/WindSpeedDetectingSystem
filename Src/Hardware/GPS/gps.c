#include "gps.h"
#include "usart.h"
#include "1602.h"
#include "GSM.h"

void ErrorLog(int num)
{
	while (1)
	{
//	  	printf("ERROR%d\r\n",num);
	}
}

void ParseGPSBuffer(void)
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
//		printf("**************\r\n");
//		printf(Save_Data.GPS_Buffer);
		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					ErrorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					ErrorLog(2);	//解析错误
				}
			}
		}
	}
}

void PrintGPSBuffer(void)
{
  uint8_t i = 0;
  uint8_t GPSDataDisBuf[16] = "P: 00N00'000E00'";
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
//		printf("Save_Data.UTCTime = ");
//		printf(Save_Data.UTCTime);
//		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
      // P: 33N15'112E15'
      GPSDataDisBuf[3] = Save_Data.latitude[0];
      GPSDataDisBuf[4] = Save_Data.latitude[1];
      GPSDataDisBuf[5] = Save_Data.N_S[0];
      GPSDataDisBuf[6] = Save_Data.latitude[2];
      GPSDataDisBuf[7] = Save_Data.latitude[3];
      
      GPSDataDisBuf[9]  = Save_Data.longitude[0];
      GPSDataDisBuf[10] = Save_Data.longitude[1];
      GPSDataDisBuf[11] = Save_Data.longitude[2];
      GPSDataDisBuf[12] = Save_Data.E_W[0];
      GPSDataDisBuf[13] = Save_Data.longitude[3];
      GPSDataDisBuf[14] = Save_Data.longitude[4];
      for(i = 0;i < 16;i++){
        lcd_char_write(i, 0, GPSDataDisBuf[i]);
      }
//			printf("Save_Data.latitude = ");
//			printf(Save_Data.latitude);
//			printf("\r\n");

//			printf("Save_Data.N_S = ");
//			printf(Save_Data.N_S);
//			printf("\r\n");

//			printf("Save_Data.longitude = ");
//			printf(Save_Data.longitude);
//			printf("\r\n");

//			printf("Save_Data.E_W = ");
//			printf(Save_Data.E_W);
//			printf("\r\n");
		}
		else
		{
//			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}

void GPS_Init(void){
  clrStruct();
  USART2_Init(9600);
}
