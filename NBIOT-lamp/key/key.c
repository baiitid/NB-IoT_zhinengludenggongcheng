
#include "stm32l1xx_hal.h"
#include "key.h"

//#define	KEY2_GPIO	GPIOC
//#define	KEY2_GPIO_PIN	GPIO_PIN_13
//#define	KEY3_GPIO	GPIOB
//#define	KEY3_GPIO_PIN	GPIO_PIN_1
//#define	KEY4_GPIO	GPIOB
//#define	KEY4_GPIO_PIN	GPIO_PIN_5

//#define KEY2 2
//#define KEY3 3
//#define KEY4 4

//#define KEY_UP			1
//#define KEY_DOWN		0

void keys_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = KEY2_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(KEY2_GPIO, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = KEY3_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(KEY3_GPIO, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = KEY4_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(KEY4_GPIO, &GPIO_InitStruct);
}

unsigned char key_even(unsigned char keyNum)
{
	unsigned char keyState;
	switch(keyNum)
	{
		case 2:			
			if( ( keyState = HAL_GPIO_ReadPin(KEY2_GPIO, KEY2_GPIO_PIN)) == KEY_DOWN)
			{
				HAL_Delay(10);
				if( ( keyState = HAL_GPIO_ReadPin(KEY2_GPIO, KEY2_GPIO_PIN)) == KEY_DOWN)
				{
					keyState = 0;
					while(!HAL_GPIO_ReadPin(KEY2_GPIO, KEY2_GPIO_PIN));
				}
				else keyState = 1;
			}
			else keyState = 1;
			break;
			
		case 3:
			if( ( keyState = HAL_GPIO_ReadPin(KEY3_GPIO, KEY3_GPIO_PIN)) == KEY_DOWN)
			{
				HAL_Delay(10);
				if( ( keyState = HAL_GPIO_ReadPin(KEY3_GPIO, KEY3_GPIO_PIN)) == KEY_DOWN)
				{
					keyState = 0;
					while(!HAL_GPIO_ReadPin(KEY3_GPIO, KEY3_GPIO_PIN));
				}
				else keyState = 1;
			}
			else keyState = 1;
			break;
			
		case 4:
			if( ( keyState = HAL_GPIO_ReadPin(KEY4_GPIO, KEY4_GPIO_PIN)) == KEY_DOWN)
			{
				HAL_Delay(10);
				if( ( keyState = HAL_GPIO_ReadPin(KEY4_GPIO, KEY4_GPIO_PIN)) == KEY_DOWN)
				{
					keyState = 0;
					while(!HAL_GPIO_ReadPin(KEY4_GPIO, KEY4_GPIO_PIN));
				}
				else keyState = 1;
			}
			else keyState = 1;
			break;
			
			default:
				keyState = 2;
				break;
	}
	return keyState;
}

