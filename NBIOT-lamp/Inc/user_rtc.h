#ifndef _USER_RTC_H_
#define _USER_RTC_H_
#include "stm32l1xx_hal.h"


void printf_data_and_time(void);
void set_DateAndTime(int iY,int iM,int iD,int iH,int iMin,int iS);
void time_cal(uint8_t timeSign,uint8_t *nian, uint8_t *yue, uint8_t *ri, uint8_t *shi);


#endif   //_USER_RTC_H_
