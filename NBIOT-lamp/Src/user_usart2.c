#include "user_usart2.h"

/****************************************************************
 *需要定义的全局变量
 ****************************************************************/
uint8_t usart2_data_fifo_buf[USART2_DATA_FIFO_BUF_SIZE];  

unsigned int usart2_data_fifo_buf_in = 0;
unsigned int usart2_data_fifo_buf_out = 0;
volatile uint8_t usart2_data_frame_put_index = 0;
volatile uint8_t usart2_data_frame_get_index = 0;

/****************************************************************
 *Function: unsigned int usart2_data_rx_fifo_len(void)
 *Describe: 获取数据缓存区已存储的数据量
 *Input   ：NONE
 *return  ：返回数据缓存区已存储的数据量
 ****************************************************************/
unsigned int usart2_data_fifo_len(void)
{
	return (usart2_data_fifo_buf_in - usart2_data_fifo_buf_out);
}

/****************************************************************
 *Function: uint8_t usart2_data_fifo_is_empty(void)
 *Describe: 判断数据缓存区是否为空
 *Input   ：NONE
 *return  ：有数据：0   无数据：1
 ****************************************************************/
uint8_t usart2_data_fifo_is_empty(void)
{
	return (usart2_data_fifo_buf_in == usart2_data_fifo_buf_out);
}

/****************************************************************
 *Function: uint8_t usart2_data_fifo_is_full(void)
 *Describe: 判断数据缓存区是否溢出
 *Input   ：NONE
 *return  ：没溢出：0   溢出：1
 ****************************************************************/
uint8_t usart2_data_fifo_is_full(void)
{
	return (usart2_data_fifo_len() > USART2_DATA_FIFO_BUF_SIZE_MASK);
}

/****************************************************************
 *Function: void usart2_data_fifo_clean(void)
 *Describe: 清空数据缓存区
 *Input   ：NONE
 *return  ：NONE
 ****************************************************************/
void usart2_data_fifo_clean(void)
{
	if(!usart2_data_fifo_is_empty())
		usart2_data_fifo_buf_out = usart2_data_fifo_buf_in;
}

/****************************************************************
 *Function: int is_has_usart2_data_frame(void)
 *Describe: 判断数据缓存区是否有帧数据
 *Input   ：NONE
 *return  ：有：1   无：0
 ****************************************************************/
int is_has_usart2_data_frame(void)
{
	return (usart2_data_frame_put_index != usart2_data_frame_get_index);
}

/****************************************************************
 *Function: int usart2_data_fifo_put(uint8_t usart2_data)
 *Describe: 向数据缓存区写入一字节数据
 *Input   ：usart2_data(要写入的一字节数据)
 *return  ：成功：0 失败：-1
 ****************************************************************/
int usart2_data_fifo_put(uint8_t usart2_data)
{
	if(usart2_data_fifo_is_full()){
		//printf("usart2_data_fifo_is_full\r\n");
		return -1;
	}else{
		//printf("index:%d, usart2_data:%d\n", usart2_data_fifo_buf_in & USART2_DATA_FIFO_BUF_SIZE_MASK, usart2_data);
		usart2_data_fifo_buf[usart2_data_fifo_buf_in & USART2_DATA_FIFO_BUF_SIZE_MASK] = usart2_data;
		usart2_data_fifo_buf_in++;

		if(usart2_data == USART2_DATA_FRAME_BACKSPACE_CHAR){
			usart2_data_frame_put_index++;
		}

		return 0;
	}
}

/****************************************************************
 *Function: int usart2_data_fifo_get(uint8_t* usart2_data)
 *Describe: 从数据缓存区读出一字节数据
 *Input   ：usart2_data(读出的一字节数据的存储地址)
 *return  ：成功：0 失败：-1
 ****************************************************************/
int usart2_data_fifo_get(uint8_t* usart2_data)
{
	if(usart2_data_fifo_is_empty()){
		return -1;
	}else{
		*usart2_data = usart2_data_fifo_buf[usart2_data_fifo_buf_out & USART2_DATA_FIFO_BUF_SIZE_MASK];
		usart2_data_fifo_buf_out++;
		
		if(*usart2_data == USART2_DATA_FRAME_BACKSPACE_CHAR){
			usart2_data_frame_get_index++;
		}

		return 0;
	}
}

/****************************************************************
 *Function: int usart2_data_fifo_write(uint8_t* usart2_datas, int len)
 *Describe: 向数据缓存区写入len字节数据
 *Input   ：usart2_data(数据的首地址) len(数据的长度)
 *return  ：写入数据的长度
 ****************************************************************/
 int usart2_data_fifo_write(uint8_t* usart2_datas, int len)
{
	int i;

	for(i = 0; i < len; i++){
		if(usart2_data_fifo_put(usart2_datas[i]) == -1)
			break;
	}
	return i;
}

/****************************************************************
 *Function: int usart2_data_fifo_read(uint8_t* usart2_datas, int len)
 *Describe: 从数据缓存区读出len字节数据
 *Input   ：usart2_data(数据存储的首地址) len(数据的长度)
 *return  ：读出数据的长度
 ****************************************************************/
int usart2_data_fifo_read(uint8_t* usart2_datas, int len)
{
	int i;

	for(i = 0; i < len; i++){
		if(usart2_data_fifo_get(usart2_datas+i) == -1)
			break;
	}
	
	return i;
}

