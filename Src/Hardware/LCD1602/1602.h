#define      LCD1602_RS0           GPIOB->BRR  = GPIO_Pin_10//0x00000004 //�͵�ƽ  PB.15
#define      LCD1602_RW0           GPIOB->BRR  = GPIO_Pin_9//0x00000010 //�͵�ƽ  PB.14
#define      LCD1602_EN0           GPIOB->BRR  = GPIO_Pin_8//0x00000040 //�͵�ƽ  PB.13
 						    
//============================�˿�λ����/����Ĵ���=========================================//
#define      LCD1602_RS1           GPIOB->BSRR = GPIO_Pin_10//0x00000004 //�ߵ�ƽ  PB.15
#define      LCD1602_RW1           GPIOB->BSRR = GPIO_Pin_9//0x00000010 //�ߵ�ƽ  PB.14
#define      LCD1602_EN1           GPIOB->BSRR = GPIO_Pin_8 //0x00000040 //�ߵ�ƽ  PB.13
 
#define      lcd_data_port              GPIOA->ODR//���ݶ˿� PA0-PA7
void lcd_char_write(unsigned int x_pos,unsigned int y_pos,unsigned char lcd_dat);
void lcd_system_reset(void);
void lcd_command_write( unsigned char command);
void lcd_busy_wait(void);
void lcd_delay( unsigned char ms);
void GPIO_InitStructReadtempCmd(void);
