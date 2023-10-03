#ifndef _USER_OLED_H_
#define _USER_OLED_H_
#include "stm32l1xx_hal.h"

#define UNLINK 0 		//δ����
#define LINKING 1		//������
#define LINKED 2		//������

#define MANUAL 0 		//�ֶ�
#define AUTO 1		//�Զ�

#define LIGHT_OPEN 0 		//��
#define LIGHT_CLOSE 1		//��

void oled_display_information(void);
void oled_display_illumination(float value);
void oled_show_mode(uint8_t status);
void oled_display_connection_status(uint8_t status);
void oled_display_light_status(uint8_t status);

#endif //_USER_OLED_H_
