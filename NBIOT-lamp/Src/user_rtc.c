#include "user_rtc.h"

extern RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

/******************************************************************************
 * FunctionName : void printf_data_and_time(void)
 * Description  : 打印rtc时间（年-月-日 时:分:秒）
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
 * Description  : 设置RTC时间
 * Parameters   : [in] iY -> 年
 *                [in] iM -> 月
 *                [in] iD -> 日
 *                [in] iH -> 时
 *                [in] iMin -> 分
 *                [in] iS -> 秒
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
 * Description  : 1.保存输入数据，将时间中的 时-8 并对判断是否对日、月、年
 *                  产生影响，若产生影响则修改相应的位置
 *
 *                2.恢复上一次保存的时间数据
 *
 * Parameters   : timeSign : 1 -> 保存输入数据，并进行处理（实现功能1）
 *                        2 -> 恢复数据（实现功能2）
 *                nian：年
 *                yue： 月
 *                ri:   日
 *                shi:  时
 * Returns      : none
*******************************************************************************/
void time_cal(uint8_t timeSign,uint8_t *nian, uint8_t *yue, uint8_t *ri, uint8_t *shi)
{
	static uint8_t timeSave[4];
	
	if(timeSign == 1)
	{
		timeSave[0] = *nian;  //存储输入参数
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
					if( ( ( *nian / 4 == 0) && ( *nian % 100 != 0 ) | ( *nian / 400 == 0 )) & ( *yue == 2 )) *ri = 28; //是否为润年
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

