#ifndef __GPS_H
#define __GPS_H

#include "stm32f10x.h"

void ErrorLog(int num);
void ParseGPSBuffer(void);
void PrintGPSBuffer(void);
void GPS_Init(void);

#endif /* __GPS_H */

