#ifndef __GSM_H
#define __GSM_H

#include "stm32f10x.h"

extern uint8_t StoreBuf[20];
extern uint8_t SendBuf[120];

void sim800c_sms_send(uint8_t *p1);
void GSM_Init(void);

#endif /* __GSM_H */

