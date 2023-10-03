#include <string.h>
#include "oled.h"
#include "user_oled.h"



/******************************************************************************
 * FunctionName : void oled_display_information(void)
 * Description  : OLED��ʾ��ʼ��Ϣ             
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void oled_display_information(void)
{
	uint8_t tmp[] = ":";
	
	//��һ�о�����ʾ���ܵ�
	OLED_ShowCHinese(40,0,0);
	OLED_ShowCHinese(56,0,1);
	OLED_ShowCHinese(72,0,2);

	//�ڶ�����ʾ����״̬��
	OLED_ShowCHinese(0 ,2,3);
	OLED_ShowCHinese(16,2,4);
	OLED_ShowCHinese(32,2,5);
	OLED_ShowCHinese(48,2,6);
	OLED_ShowString( 64,2,tmp);
	
	//��������ʾ����ǿ��:
	OLED_ShowCHinese(0 ,4,7);
	OLED_ShowCHinese(16,4,8);
	OLED_ShowCHinese(32,4,9);
	OLED_ShowCHinese(48,4,10);
	OLED_ShowString( 64,4,tmp);
	
	//��������ʾ�ƹ�״̬:
	OLED_ShowCHinese(0 ,6,11);
	OLED_ShowCHinese(16,6,12);
	OLED_ShowCHinese(32,6,13);
	OLED_ShowCHinese(48,6,14);
	OLED_ShowString( 64,6,tmp);
}

/******************************************************************************
 * FunctionName : void oled_display_illumination(float value)
 * Description  : ��OLED����ʾ����ǿ����ֵ      
 * Parameters   : [in] value
 * Returns      : none
*******************************************************************************/
void oled_display_illumination(float value)
{
	uint8_t tmp[8] = {0};
	uint8_t oledBuf[32]; 
	sprintf((char *)oledBuf, "%f     ",value); //��ʽ�� 
	memcpy(tmp, oledBuf, 7);
	OLED_ShowString(72,4,tmp); //OLED��ʾ
}

/******************************************************************************
 * FunctionName : void oled_display_illumination(float value)
 * Description  : ��OLED����ʾ��ǰģʽ
 * Parameters   : [in] value
 * Returns      : none
*******************************************************************************/
void oled_show_mode(uint8_t status)
{
	if(status == MANUAL)
		OLED_ShowString(112,6,(uint8_t *)"M");
	else if(status == AUTO)
		OLED_ShowString(112,6,(uint8_t *)"A");
	
	
}

/******************************************************************************
 * FunctionName : void oled_display_connection_status(uint8_t status)
 * Description  : ��OLED����ʾ����ƽ̨������״̬
 * Parameters   : [in] status ��
 *															UNLINK  -> δ����
 *                              LINKING -> ������
 *                              LINKED  -> ������
 * Returns      : none
*******************************************************************************/

void oled_display_connection_status(uint8_t status)
{
	switch(status)
	{
		case UNLINK : {
			OLED_ShowCHinese(72,2,16);
			OLED_ShowCHinese(88,2,17);
			OLED_ShowCHinese(104,2,18);
			break;
		}
		case LINKING : {
			OLED_ShowCHinese(72,2,17);
			OLED_ShowCHinese(88,2,18);
			OLED_ShowCHinese(104,2,19);
			break;
		}
		case LINKED : {
			OLED_ShowCHinese(72,2,15);
			OLED_ShowCHinese(88,2,17);
			OLED_ShowCHinese(104,2,18);
			break;
		}
	}
}

/******************************************************************************
 * FunctionName : void oled_display_light_status(uint8_t status)
 * Description  : ��OLED����ʾ�ƹ��״̬
 * Parameters   : [in] status ��
 *															LIGHT_OPEN  -> ��
 *                              LIGHT_CLOSE -> ��
 * Returns      : none
*******************************************************************************/

void oled_display_light_status(uint8_t status)
{
	switch(status)
	{
		case LIGHT_OPEN : {
			OLED_ShowCHinese(72,6,20);
			break;
		}
		case LIGHT_CLOSE : {
			OLED_ShowCHinese(72,6,21);
			break;
		}
	}
}

