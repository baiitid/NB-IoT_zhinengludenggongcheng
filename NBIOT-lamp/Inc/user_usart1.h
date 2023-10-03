#ifndef _USER_USART1_H_
#define _USER_USART1_H_
#include "stm32l1xx_hal.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define USART1_DATA_FRAME_NEXTLINE_CHAR	'\n'
#define USART1_DATA_FRAME_BACKSPACE_CHAR	'\r'

#define USART1_DATA_FIFO_BUF_SIZE		128 //it must be pow(2, x) result 
#define USART1_DATA_FIFO_BUF_SIZE_MASK		(USART1_DATA_FIFO_BUF_SIZE-1)


uint32_t usart1_data_fifo_len(void);
uint8_t usart1_data_fifo_is_empty(void);
uint8_t usart1_data_fifo_is_full(void);
void usart1_data_fifo_clean(void);
int is_has_usart1_data_frame(void);
int usart1_data_fifo_put(uint8_t usart1_data);
int usart1_data_fifo_get(uint8_t* usart1_data);
int usart1_data_fifo_write(uint8_t* usart1_datas, int len);
int usart1_data_fifo_read(uint8_t* usart1_datas, int len);


#endif  //_USER_USART1_H
