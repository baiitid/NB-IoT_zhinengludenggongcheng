#include "user_rtc.h"

extern RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

/******************************************************************************
 * FunctionName : void printf_data_and_time(void)
 * Description  : ��ӡrtcʱ�䣨��-��-�� ʱ:��:�룩
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void printf_data_and_time(void)
{	
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	
	printf("20%02d/%02d/%02d/ %02d:%02d:%02d\r\n", sDate.Year, sDate.Month, sDate.Date, sTime.Hours, sTime.Minutes, sTime.Seconds);
}


/******************************************************************************
 * FunctionName : void set_DateAndTime(int iY,int iM,int iD,int iH,int iMin,int iS)
 * Description  : ����RTCʱ��
 * Parameters   : [in] iY -> ��
 *                [in] iM -> ��
 *                [in] iD -> ��
 *                [in] iH -> ʱ
 *                [in] iMin -> ��
 *                [in] iS -> ��
 * Returns      : none
*******************************************************************************/
void set_DateAndTime(int iY,int iM,int iD,int iH,int iMin,int iS) 
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
	
    iY=iY-48;
    sTime.Hours =iH;
    sTime.Minutes =iMin;
    sTime.Seconds =iS;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;//====
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;//====
		HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	
    sDate.WeekDay = RTC_WEEKDAY_MONDAY;//====
    sDate.Month = iM;//RTC_MONTH_JANUARY;
    sDate.Date = iD;
    sDate.Year = iY;
	
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

}

/******************************************************************************
 * FunctionName : void time_cal(uint8_t timeSign,uint8_t *nian, uint8_t *yue, 
 *                                                    uint8_t *ri, uint8_t *shi)
 *
 * Description  : 1.�����������ݣ���ʱ���е� ʱ-8 �����ж��Ƿ���ա��¡���
 *                  ����Ӱ�죬������Ӱ�����޸���Ӧ��λ��
 *
 *                2.�ָ���һ�α����ʱ������
 *
 * Parameters   : timeSign : 1 -> �����������ݣ������д���ʵ�ֹ���1��
 *                        2 -> �ָ����ݣ�ʵ�ֹ���2��
 *                nian����
 *                yue�� ��
 *                ri:   ��
 *                shi:  ʱ
 * Returns      : none
*******************************************************************************/
void time_cal(uint8_t timeSign,uint8_t *nian, uint8_t *yue, uint8_t *ri, uint8_t *shi)
{
	static uint8_t timeSave[4];
	
	if(timeSign == 1)
	{
		timeSave[0] = *nian;  //�洢�������
		timeSave[1] = *yue;
		timeSave[2] = *ri;
		timeSave[3] = *shi;
		
		if( *shi < 8)
		{
			*shi = 24 + *shi - 8;
			*ri -= 1;
			if( *ri == 0)
			{
				if( ( *yue == 1) | ( *yue == 3) | ( *yue == 5) | ( *yue == 7) | ( *yue == 8) | ( *yue == 10) | ( *yue == 12)) *ri = 31;  
				else
				{
					if( ( ( *nian / 4 == 0) && ( *nian % 100 != 0 ) | ( *nian / 400 == 0 )) & ( *yue == 2 )) *ri = 28; //�Ƿ�Ϊ����
					else if( *yue == 2 ) *ri = 27;
					else *ri = 30;
				}
				*yue -= 1;
				if( *yue == 0)
				{
					*yue = 12;
					*nian -= 1; 
				}
			}
		}
		else *shi -= 8;
	//	printf("timeSign=%d,nian=%d,yue=%d,ri=%d,shi=%d \n",timeSign,*nian,*yue,*ri,*shi);
	}
	else
	{
		*nian = timeSave[0];
		*yue = timeSave[1];
		*ri = timeSave[2];
		*shi = timeSave[3];
	//	printf("timeSign=%d,nian=%d,yue=%d,ri=%d,shi=%d \n",timeSign,*nian,*yue,*ri,*shi);
	}
}

