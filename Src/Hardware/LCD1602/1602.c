#include "1602.h"
#include "delay.h"
//1602初始化
void lcd_delay( unsigned char ms) /*LCD1602 延时*/
{
  unsigned char j;
  while (ms--) {
    for (j = 0; j < 250; j++) {
      ;
    }
  }
}

void GPIO_InitStructReadtempCmd(void)
{
  GPIO_InitTypeDef	GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void lcd_busy_wait(void) /*LCD1602 忙等待*/
{
  u8 sta;
  LCD1602_RS0;
  LCD1602_RW1;
  lcd_data_port = 0xff;
  do {
    LCD1602_EN1;
    lcd_delay(5);
    sta = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
    LCD1602_EN0;
  } while (sta & 0x80);

}

void lcd_command_write( unsigned char command) /*LCD1602 命令字写入*/
{
  lcd_busy_wait();
  LCD1602_RS0;
  LCD1602_RW0;
  LCD1602_EN0;
  lcd_data_port = command;
  LCD1602_EN1;
  LCD1602_EN0;
}


void lcd_system_reset(void) /*LCD1602 初始化*/
{
  lcd_delay(20);
  lcd_command_write(0x38);
  lcd_delay(100);
  lcd_command_write(0x38);
  lcd_delay(50);
  lcd_command_write(0x38);
  lcd_delay(10);
  lcd_command_write(0x0c);
  lcd_command_write(0x06);
  lcd_command_write(0x01);
  lcd_command_write(0x80);
  lcd_delay(100);
}

void lcd_char_write(unsigned int x_pos, unsigned int y_pos, unsigned char lcd_dat) /*LCD1602 字符写入*/
{
  x_pos &= 0x0f;
  y_pos &= 0x01;
  if (y_pos == 1) x_pos += 0x40;
  x_pos += 0x80;
  lcd_command_write(x_pos);
  lcd_busy_wait();
  LCD1602_RS1;
  LCD1602_RW0;
  LCD1602_EN0;
  lcd_data_port = lcd_dat;
  LCD1602_EN1;
  LCD1602_EN0;
}

