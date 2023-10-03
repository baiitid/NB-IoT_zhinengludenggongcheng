#include <stdio.h>
#include <string.h>
#include "user_usart2.h"
#include "user_cloud.h"
#include "user_rtc.h"

extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;


/******************************************************************************
 * FunctionName : void send_AT_command(char* format, ...)
 * Description  : 发送AT指令到NB模块
 * Parameters   : 
 * Returns      : none
*******************************************************************************/
void send_AT_command(char* format, ...)
{
    va_list va;
		char nbiot_AT_cmd_buff[128];
    uint8_t tx_count = 0, tx_size;

    va_start(va, format);
    vsprintf(nbiot_AT_cmd_buff, format, va);
    va_end(va);

    //HAL_Delay(200);
    //printf("SEND:%s", nbiot_AT_cmd_buff);

    tx_size = strlen(nbiot_AT_cmd_buff);

    while(tx_count < tx_size) {
        HAL_UART_Transmit(&huart2, (uint8_t*)&nbiot_AT_cmd_buff[tx_count], 1, 10);
        tx_count++;
    }
}

/******************************************************************************
 * FunctionName : int wait_answer(char *str)
 * Description  : 等待接收到NB模块相应的指定字符串
 * Parameters   : [in] str
 * Returns      :  0 -> 接收到指定字符串
 *                -1 -> 未接收到指定字符串
*******************************************************************************/
int wait_answer(char *str)
{
	uint8_t tmp[128] = {0};
	uint32_t len;
	int j = 0;
	
	for(j=0;j<10;j++)
	{
		HAL_Delay(1000);
		len = usart2_data_fifo_len();
		if(len > 0)
		{
			usart2_data_fifo_read(tmp, len);
			printf("USART2:%s\r\n",tmp);
			if(strstr((char *)tmp, str) != NULL)
			{
				printf("Find (%s)\r\n",str);
				return 0;
			}
			memset(tmp, 0, sizeof(tmp));
		}
	}
	printf("Can't find (%s)\r\n",str);
	return -1;
}

/******************************************************************************
 * FunctionName : void rcv_data_deal(void)
 * Description  : 接收数据处理
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
uint8_t rcv_data_deal(void)
{
	static uint8_t tmp[128] = {0};
	int i,ret = 0;
	char *prt;
	
	for(i=0;i<128;i++)
	{
		if(usart2_data_fifo_get(&tmp[i]) < 0) break;
		if(tmp[i] == USART2_DATA_FRAME_BACKSPACE_CHAR)
		{
			ret = RCV_OK;
			printf("USART2:%s\r\n",tmp);
			
				
			//连接云平台成功
			if(strstr((char *)tmp, "AT+MLWEVTIND=3") != NULL)
				ret = LINK_OK;    //LINK
			
			//收到时间数据
			if((prt = strstr((char *)tmp, "+CCLK")) != NULL)
			{
				int iy = (prt[6] - '0')*10 + (prt[7] - '0');
				int im = (prt[9] - '0')*10 + (prt[10] - '0');
				int id = (prt[12] - '0')*10 + (prt[13] - '0');
				int ih =(prt[15] - '0')*10 + (prt[16] - '0') ;
				int imin = (prt[18] - '0')*10 + (prt[19] - '0');
				int is = (prt[21] - '0')*10 + (prt[22] - '0');
				
				//time_cal(1, (uint8_t*)&iy, (uint8_t*)&im, (uint8_t*)&id, (uint8_t*)&ih);
				
				printf("%d-%d-%d %d:%d:%d\r\n",2000+iy, im, id, ih, imin, is);
				
				set_DateAndTime(2000+iy, im, id, ih, imin ,is);
				
				ret = TIME_OK;    //TIME_OK
			
			}
			
			
			//接收到云平台控制指令
			if((prt = strstr((char *)tmp, "+NNMI:8")) != NULL)
			{
				//+NNMI:8,4A00020000000300
				//4A identifier
				//00 msgType
				//02 serviceId
				//00 cmd
				//00 hasMore
				//0000 mid
				//00 控制值
				printf("prt[13]:%c\r\n",prt[13]);
				printf("prt[23]:%c\r\n",prt[23]);
				if(prt[13] == '2')
				{
					if(prt[23] == '1')
						ret = CONTROL_OPEN;  //LIGHT_OPEN
					else
						ret = CONTROL_CLOSE;  //LIGHT_CLOSE
				}
			}
			
			
			

			memset(tmp, 0,sizeof(tmp));
			break;
		}
	}
	
	return ret;
}

/******************************************************************************
 * FunctionName : void nb_reset(void)
 * Description  : NB模块复位
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void nb_reset(void)
{
	send_AT_command("AT+NRB\r\n");
}


/******************************************************************************
 * FunctionName : void wait_nbiot_start(void)
 * Description  : 等待NB模块启动
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void wait_nbiot_start(void)
{
	int timeOut = 0;
	printf("waite NBIOT Start\r\n");
	while(1)
	{
		HAL_Delay(1000);
		if(wait_answer("OK") == 0)
		{
			printf("NBIOT Start\r\n");
			break;
		}
		if(timeOut > 10)
		{
			timeOut = 0;
			nb_reset();
			printf("waite NBIOT Start\r\n");
		}
		timeOut++;
	}
	
}

/******************************************************************************
 * FunctionName : void nbiot_config(void)
 * Description  : NB模块配置
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void nbiot_config(void)  
{  //开启NB-IoT芯片所有功能    
    send_AT_command("AT+CFUN=%d\r\n",1);   
    wait_answer("OK");          
    //查询信号连接状态
    send_AT_command("AT+CSCON=%d\r\n", 0);  
    wait_answer("OK");        
    //打开网络注册和位置信息的主动上报结果码 0关闭 1 注册并上报 2 注册并上报位置信息
    send_AT_command("AT+CEREG=%d\r\n", 2);  
    wait_answer("OK");        
    //开启下行数据通知
    send_AT_command("AT+NNMI=%d\r\n", 1);  
    wait_answer("OK");        
    //打开与核心网的连接  1是打开  0是关闭
    send_AT_command("AT+CGATT=%d\r\n", 1);  
    wait_answer("OK");  
}



/******************************************************************************
 * FunctionName : void link_server(void)
 * Description  : NB模块连接服务器
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void link_server(void)
{
	//link sever
 send_AT_command("AT+NCDP=%s,%d\r\n", "117.60.157.137", 5683);  
    wait_answer("OK"); 

	//wait OK

}

/******************************************************************************
 * FunctionName : void get_time_from_server()
 * Description  : 从服务器获取时间
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void get_time_from_server(void)
{
	//Get time
	 send_AT_command("AT+CCLK?\r\n"); 
	
}

/******************************************************************************
 * FunctionName : void link_server(void)
 * Description  : NB模块发送数据到云平台
 * Parameters   : [in] illumination
 *                [in] light_status
 * Returns      : none
*******************************************************************************/
void send_data_to_cloud(int illumination,uint8_t light_status)
{
	uint8_t send_buf[128] = {0};
	
	RTC_TimeTypeDef gTime;
	RTC_DateTypeDef gDate;
	
	//时间校准
	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
	//time_cal(1, &gDate.Year, &gDate.Month, &gDate.Date, &gTime.Hours);
	
	
	
	sprintf((char *)send_buf, "\
%02X%02X%02X\
%02X%02X%02X\
%02X%02X\
%02X%02X%02X%02X%02X%02X%02X%02X\
",
	0x4a,0x00,0x00,
	0x01,(illumination & 0xff00) >> 8, (illumination & 0x00ff),
	0x02, light_status,
	0x04,20, gDate.Year, gDate.Month, gDate.Date, gTime.Hours, gTime.Minutes, gTime.Seconds);

	printf("send sensors data:AT+NMGS=%d,%s\r\n", (strlen((char *)send_buf)/2), send_buf);
	send_AT_command("AT+NMGS=%d,%s\r\n", (strlen((char *)send_buf)/2), send_buf);

}



