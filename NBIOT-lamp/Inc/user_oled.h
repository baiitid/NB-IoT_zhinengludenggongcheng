#ifndef _USER_OLED_H_
#define _USER_OLED_H_
#include "stm32l1xx_hal.h"

#define UNLINK 0 		//未连接
#define LINKING 1		//连接中
#define LINKED 2		//已连接

#define MANUAL 0 		//手动
#define AUTO 1		//自动

#define LIGHT_OPEN 0 		//开
#define LIGHT_CLOSE 1		//关

void oled_display_information(void);
void oled_display_illumination(float value);
void oled_show_mode(uint8_t status);
void oled_display_connection_status(uint8_t status);
void oled_display_light_status(uint8_t status);

#endif //_USER_OLED_H_
