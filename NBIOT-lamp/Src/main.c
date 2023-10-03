/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "oled.h"
#include "user_oled.h"
#include "user_usart1.h"
#include "user_usart2.h"
#include "user_cloud.h"
#include "key.h"



ADC_HandleTypeDef hadc;

RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

uint8_t usart1RxBuf;
uint8_t usart2RxBuf;



/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);

//用于实现USART1 printf()输出
int fputc(int ch, FILE *f)
{ 
  HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 10);
  return(ch);
}

/******************************************************************************
 * FunctionName : void get_illumination_value()
 * Description  : 获取光照强度数值
 * Parameters   : none
 * Returns      : illumination_value
*******************************************************************************/
float get_illumination_value()
{
	float adcValue;
	float illumination_value;

	
	//启动ADC
	HAL_ADC_Start(&hadc);  
	
	//等待采集完成
	HAL_ADC_PollForConversion(&hadc,10); 
	
	//获取ADC采集的数据
	adcValue = HAL_ADC_GetValue(&hadc);

	//将采集到的数据转换为电压值(单位:v)
	adcValue = adcValue * 3.3 / 4096.0;

	//将电压值转换为光照强度
	illumination_value = pow(10,((1.78-log10(33/adcValue -10))/0.6));
	
	//在OLED上显示数据
	oled_display_illumination(illumination_value);
	
	//停止ADC
	HAL_ADC_Stop(&hadc); 
	
	return illumination_value;
}

/******************************************************************************
 * FunctionName : void control_light(uint8_t status)
 * Description  : 控制灯的亮灭
 * Parameters   : [in] status
 * Returns      : none
*******************************************************************************/
void control_light(uint8_t status)
{
	if(status == LIGHT_OPEN)
	{
		HAL_GPIO_WritePin(Light_GPIO_Port, Light_Pin, GPIO_PIN_SET);
		oled_display_light_status(LIGHT_OPEN);
	}
	else
	{
		HAL_GPIO_WritePin(Light_GPIO_Port, Light_Pin, GPIO_PIN_RESET);
		oled_display_light_status(LIGHT_CLOSE);
	}
}

/******************************************************************************
 * FunctionName : void automatic_mode()
 * Description  : 自动模式
 * Parameters   : [in] status
 * Returns      : none
*******************************************************************************/
void automatic_mode(int value, int *light_flag)
{
			static int now_ill_value;
			if((value < 3) && (*light_flag == 0))
			{
				control_light(LIGHT_OPEN);
				*light_flag = 1;
				HAL_Delay(10);
				now_ill_value = (int)get_illumination_value();
			}
			else if((now_ill_value+1 < value) && (*light_flag == 1))
			{
				control_light(LIGHT_CLOSE);
				*light_flag = 0;
			}

}
	

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
	
	
  //OLED初始化
	OLED_Init();
	//按键初始化
	keys_init();
	
	//OLED显示初始信息
	oled_display_information();
	oled_display_connection_status(LINKING);
	oled_display_light_status(LIGHT_CLOSE);
	oled_show_mode(MANUAL);
	
	//开启USART1中断接收
	HAL_UART_Receive_IT(&huart1, &usart1RxBuf, 1); 
	//开启USART2中断接收
	HAL_UART_Receive_IT(&huart2, &usart2RxBuf, 1);

	//等待NB模块启动
	wait_nbiot_start();
	
	//NB模块配置
	nbiot_config();
	
	//连接服务器
	link_server();
	
	int i, ret, ill_value, lightStatus, link_flag = 0, send_count;
	uint8_t mod_flag=0, light_flag=0;
  while (1)
  {
		//1.5S采集并发送一次数据
		if(i++ > 14)
		{
			i = 0;
			//获取光照值
			ill_value = (int)get_illumination_value();
			
			//自动模式下，光照强度小于3会自动开灯
			if(mod_flag == 1)
			{
				automatic_mode(ill_value, &lightStatus);
			}
			
			if(link_flag < 2)
			{
				//获取时间
				get_time_from_server();

			}
			else if(link_flag == 2)
			{
				//发送数据到云平台
				send_data_to_cloud( ill_value, lightStatus);
				send_count++;
			}	
		}
		
		//接收数据处理
		ret = rcv_data_deal();
		switch(ret)
		{
			//LINK OK
			case LINK_OK : {
				link_flag = 1;
				break;
			}
			//get time OK
			case TIME_OK : {
				oled_display_connection_status(LINKED);
				link_flag = 2;
				break;
			}
			//RCV OK
			case RCV_OK : {
				send_count = 0;
				break;
			}
			//lamp OPEN
			case CONTROL_OPEN : {
				control_light(LIGHT_OPEN);
				lightStatus = 1;
				break;
			}
			//lamp CLOSE
			case CONTROL_CLOSE : {
				control_light(LIGHT_CLOSE);
				lightStatus = 0;
				break;
			}
			
		}
			
		//从新开启USART2中断
		if(send_count >= 3)
			HAL_UART_Receive_IT(&huart2, &usart2RxBuf, 1);
		
		HAL_Delay(100);
		
		//KEY2按键控制灯
		if(key_even(KEY2) == KEY_DOWN)
		{
			if(light_flag == 0)
			{
				control_light(LIGHT_OPEN);
				lightStatus = 1;
				light_flag = 1;
			}
			else
			{
				control_light(LIGHT_CLOSE);
				lightStatus = 0;
				light_flag = 0;
			}
			
		}
		
		//KEY3按键选择模式
		if(key_even(KEY3) == KEY_DOWN)
		{
			
			if(mod_flag == 0)
			{
				printf("Enter automatic mode\r\n");
				oled_show_mode(AUTO);
				mod_flag = 1;
			}
			else
			{
				printf("Enter manual mode\r\n");
				oled_show_mode(MANUAL);
				mod_flag = 0;
			}
		}
		
		
		
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.LowPowerAutoWait = ADC_AUTOWAIT_DISABLE;
  hadc.Init.LowPowerAutoPowerOff = ADC_AUTOPOWEROFF_DISABLE;
  hadc.Init.ChannelsBank = ADC_CHANNELS_BANK_A;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_4CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /**Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /**Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Light_GPIO_Port, Light_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Light_Pin */
  GPIO_InitStruct.Pin = Light_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Light_GPIO_Port, &GPIO_InitStruct);

}

//当产生串口中断后，最终会跳到这
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)  //判断是哪个串口产生的中断
	{
		usart1_data_fifo_put(usart1RxBuf); //向USART1串口缓冲区写入数据
		HAL_UART_Receive_IT(&huart1, &usart1RxBuf, 1);
	}

	if(huart == &huart2)
	{
		usart2_data_fifo_put(usart2RxBuf); //向USART2串口缓冲区写入数据
		HAL_UART_Receive_IT(&huart2, &usart2RxBuf, 1);
	}
}

//当串口出错，跳转到此
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	printf("UART Error:%x\r\n",huart->ErrorCode);  //报告错误编号
	huart->ErrorCode = HAL_UART_ERROR_NONE;
	
	if(huart == &huart1){
	HAL_UART_Receive_IT(&huart1, &usart1RxBuf, 1);  //重新打开USART1接收中断
	}

	if(huart == &huart2){
	HAL_UART_Receive_IT(&huart2, &usart2RxBuf, 1);  //重新打开USART2接收中断
	}
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
