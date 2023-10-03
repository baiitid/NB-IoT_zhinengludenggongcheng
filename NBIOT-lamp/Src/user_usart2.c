#include "user_usart2.h"

/****************************************************************
 *��Ҫ�����ȫ�ֱ���
 ****************************************************************/
uint8_t usart2_data_fifo_buf[USART2_DATA_FIFO_BUF_SIZE];  

unsigned int usart2_data_fifo_buf_in = 0;
unsigned int usart2_data_fifo_buf_out = 0;
volatile uint8_t usart2_data_frame_put_index = 0;
volatile uint8_t usart2_data_frame_get_index = 0;

/****************************************************************
 *Function: unsigned int usart2_data_rx_fifo_len(void)
 *Describe: ��ȡ���ݻ������Ѵ洢��������
 *Input   ��NONE
 *return  ���������ݻ������Ѵ洢��������
 ****************************************************************/
unsigned int usart2_data_fifo_len(void)
{
	return (usart2_data_fifo_buf_in - usart2_data_fifo_buf_out);
}

/****************************************************************
 *Function: uint8_t usart2_data_fifo_is_empty(void)
 *Describe: �ж����ݻ������Ƿ�Ϊ��
 *Input   ��NONE
 *return  �������ݣ�0   �����ݣ�1
 ****************************************************************/
uint8_t usart2_data_fifo_is_empty(void)
{
	return (usart2_data_fifo_buf_in == usart2_data_fifo_buf_out);
}

/****************************************************************
 *Function: uint8_t usart2_data_fifo_is_full(void)
 *Describe: �ж����ݻ������Ƿ����
 *Input   ��NONE
 *return  ��û�����0   �����1
 ****************************************************************/
uint8_t usart2_data_fifo_is_full(void)
{
	return (usart2_data_fifo_len() > USART2_DATA_FIFO_BUF_SIZE_MASK);
}

/****************************************************************
 *Function: void usart2_data_fifo_clean(void)
 *Describe: ������ݻ�����
 *Input   ��NONE
 *return  ��NONE
 ****************************************************************/
void usart2_data_fifo_clean(void)
{
	if(!usart2_data_fifo_is_empty())
		usart2_data_fifo_buf_out = usart2_data_fifo_buf_in;
}

/****************************************************************
 *Function: int is_has_usart2_data_frame(void)
 *Describe: �ж����ݻ������Ƿ���֡����
 *Input   ��NONE
 *return  ���У�1   �ޣ�0
 ****************************************************************/
int is_has_usart2_data_frame(void)
{
	return (usart2_data_frame_put_index != usart2_data_frame_get_index);
}

/****************************************************************
 *Function: int usart2_data_fifo_put(uint8_t usart2_data)
 *Describe: �����ݻ�����д��һ�ֽ�����
 *Input   ��usart2_data(Ҫд���һ�ֽ�����)
 *return  ���ɹ���0 ʧ�ܣ�-1
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
 *Describe: �����ݻ���������һ�ֽ�����
 *Input   ��usart2_data(������һ�ֽ����ݵĴ洢��ַ)
 *return  ���ɹ���0 ʧ�ܣ�-1
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
 *Describe: �����ݻ�����д��len�ֽ�����
 *Input   ��usart2_data(���ݵ��׵�ַ) len(���ݵĳ���)
 *return  ��д�����ݵĳ���
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
 *Describe: �����ݻ���������len�ֽ�����
 *Input   ��usart2_data(���ݴ洢���׵�ַ) len(���ݵĳ���)
 *return  ���������ݵĳ���
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
