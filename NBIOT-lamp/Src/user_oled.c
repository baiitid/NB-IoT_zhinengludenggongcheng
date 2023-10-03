#include <string.h>
#include "oled.h"
#include "user_oled.h"



/******************************************************************************
 * FunctionName : void oled_display_information(void)
 * Description  : OLED显示初始信息             
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void oled_display_information(void)
{
	uint8_t tmp[] = ":";
	
	//第一行居中显示智能灯
	OLED_ShowCHinese(40,0,0);
	OLED_ShowCHinese(56,0,1);
	OLED_ShowCHinese(72,0,2);

	//第二行显示连接状态：
	OLED_ShowCHinese(0 ,2,3);
	OLED_ShowCHinese(16,2,4);
	OLED_ShowCHinese(32,2,5);
	OLED_ShowCHinese(48,2,6);
	OLED_ShowString( 64,2,tmp);
	
	//第三行显示光照强度:
	OLED_ShowCHinese(0 ,4,7);
	OLED_ShowCHinese(16,4,8);
	OLED_ShowCHinese(32,4,9);
	OLED_ShowCHinese(48,4,10);
	OLED_ShowString( 64,4,tmp);
	
	//第四行显示灯光状态:
	OLED_ShowCHinese(0 ,6,11);
	OLED_ShowCHinese(16,6,12);
	OLED_ShowCHinese(32,6,13);
	OLED_ShowCHinese(48,6,14);
	OLED_ShowString( 64,6,tmp);
}

/******************************************************************************
 * FunctionName : void oled_display_illumination(float value)
 * Description  : 在OLED上显示光照强度数值      
 * Parameters   : [in] value
 * Returns      : none
*******************************************************************************/
void oled_display_illumination(float value)
{
	uint8_t tmp[8] = {0};
	uint8_t oledBuf[32]; 
	sprintf((char *)oledBuf, "%f     ",value); //格式化 
	memcpy(tmp, oledBuf, 7);
	OLED_ShowString(72,4,tmp); //OLED显示
}

/******************************************************************************
 * FunctionName : void oled_display_illumination(float value)
 * Description  : 在OLED上显示当前模式
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
 * Description  : 在OLED上显示与云平台的连接状态
 * Parameters   : [in] status ：
 *															UNLINK  -> 未连接
 *                              LINKING -> 连接中
 *                              LINKED  -> 已连接
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
 * Description  : 在OLED上显示灯光的状态
 * Parameters   : [in] status ：
 *															LIGHT_OPEN  -> 开
 *                              LIGHT_CLOSE -> 关
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

