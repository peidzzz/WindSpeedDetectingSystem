#ifndef __LCD1602_H
#define __LCD1602_H

#include "stm32f10x.h"

#define      LCD1602_RS0           GPIOB->BRR  = GPIO_Pin_9 //低电平  PB.9
#define      LCD1602_RW0           GPIOB->BRR  = GPIO_Pin_8 //低电平  PB.8
#define      LCD1602_EN0           GPIOB->BRR  = GPIO_Pin_7 //低电平  PB.7
 						    
#define      LCD1602_RS1           GPIOB->BSRR = GPIO_Pin_9 //高电平  PB.9
#define      LCD1602_RW1           GPIOB->BSRR = GPIO_Pin_8 //高电平  PB.8
#define      LCD1602_EN1           GPIOB->BSRR = GPIO_Pin_7 //高电平  PB.7
 
#define      lcd_data_port(x)         LCDDataPort(x) //数据端口 PA0-PA7

#define      D0     PBout(6)
#define      D1     PBout(5)
#define      D2     PBout(4)
#define      D3     PBout(3)
#define      D4     PDout(2)
#define      D5     PBout(12)
#define      D6     PBout(11)
#define      D7     PBout(10)


void lcd_char_write(unsigned int x_pos,unsigned int y_pos,unsigned char lcd_dat);
void lcd_command_write( unsigned char command);
void lcd_busy_wait(void);
void lcd_delay( unsigned char ms);
void LCDDataPort(uint8_t data);
void LCD_Init(void);

#endif /* __LCD1602_H */
