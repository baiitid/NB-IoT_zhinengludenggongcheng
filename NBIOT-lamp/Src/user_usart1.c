#include "user_usart1.h"



/****************************************************************
 *��Ҫ�����ȫ�ֱ���
 ****************************************************************/
uint8_t usart1_data_fifo_buf[USART1_DATA_FIFO_BUF_SIZE];  

unsigned int usart1_data_fifo_buf_in = 0;
unsigned int usart1_data_fifo_buf_out = 0;
volatile uint8_t usart1_data_frame_put_index = 0;
volatile uint8_t usart1_data_frame_get_index = 0;

/****************************************************************
 *Function: unsigned int usart1_data_rx_fifo_len(void)
 *Describe: ��ȡ���ݻ������Ѵ洢��������
 *Input   ��NONE
 *return  ���������ݻ������Ѵ洢��������
 ****************************************************************/
unsigned int usart1_data_fifo_len(void)
{
	return (usart1_data_fifo_buf_in - usart1_data_fifo_buf_out);
}

/****************************************************************
 *Function: uint8_t usart1_data_fifo_is_empty(void)
 *Describe: �ж����ݻ������Ƿ�Ϊ��
 *Input   ��NONE
 *return  �������ݣ�0   �����ݣ�1
 ****************************************************************/
uint8_t usart1_data_fifo_is_empty(void)
{
	return (usart1_data_fifo_buf_in == usart1_data_fifo_buf_out);
}

/****************************************************************
 *Function: uint8_t usart1_data_fifo_is_full(void)
 *Describe: �ж����ݻ������Ƿ����
 *Input   ��NONE
 *return  ��û�����0   �����1
 ****************************************************************/
uint8_t usart1_data_fifo_is_full(void)
{
	return (usart1_data_fifo_len() > USART1_DATA_FIFO_BUF_SIZE_MASK);
}

/****************************************************************
 *Function: void usart1_data_fifo_clean(void)
 *Describe: ������ݻ�����
 *Input   ��NONE
 *return  ��NONE
 ****************************************************************/
void usart1_data_fifo_clean(void)
{
	if(!usart1_data_fifo_is_empty())
		usart1_data_fifo_buf_out = usart1_data_fifo_buf_in;
}

/****************************************************************
 *Function: int is_has_usart1_data_frame(void)
 *Describe: �ж����ݻ������Ƿ���֡����
 *Input   ��NONE
 *return  ���У�1   �ޣ�0
 ****************************************************************/
int is_has_usart1_data_frame(void)
{
	return (usart1_data_frame_put_index != usart1_data_frame_get_index);
}

/****************************************************************
 *Function: int usart1_data_fifo_put(uint8_t usart1_data)
 *Describe: �����ݻ�����д��һ�ֽ�����
 *Input   ��usart1_data(Ҫд���һ�ֽ�����)
 *return  ���ɹ���0 ʧ�ܣ�-1
 ****************************************************************/
int usart1_data_fifo_put(uint8_t usart1_data)
{
	if(usart1_data_fifo_is_full()){
		return -1;
	}else{
		//printf("index:%d, usart1_data:%d\n", usart1_data_fifo_buf_in & USART1_DATA_FIFO_BUF_SIZE_MASK, usart1_data);
		usart1_data_fifo_buf[usart1_data_fifo_buf_in & USART1_DATA_FIFO_BUF_SIZE_MASK] = usart1_data;
		usart1_data_fifo_buf_in++;

		if(usart1_data == USART1_DATA_FRAME_BACKSPACE_CHAR){
			usart1_data_frame_put_index++;
		}

		return 0;
	}
}

/****************************************************************
 *Function: int usart1_data_fifo_get(uint8_t* usart1_data)
 *Describe: �����ݻ���������һ�ֽ�����
 *Input   ��usart1_data(������һ�ֽ����ݵĴ洢��ַ)
 *return  ���ɹ���0 ʧ�ܣ�-1
 ****************************************************************/
int usart1_data_fifo_get(uint8_t* usart1_data)
{
	if(usart1_data_fifo_is_empty()){
		return -1;
	}else{
		*usart1_data = usart1_data_fifo_buf[usart1_data_fifo_buf_out & USART1_DATA_FIFO_BUF_SIZE_MASK];
		usart1_data_fifo_buf_out++;
		
		if(*usart1_data == USART1_DATA_FRAME_BACKSPACE_CHAR){
			usart1_data_frame_get_index++;
		}

		return 0;
	}
}

/****************************************************************
 *Function: int usart1_data_fifo_write(uint8_t* usart1_datas, int len)
 *Describe: �����ݻ�����д��len�ֽ�����
 *Input   ��usart1_data(���ݵ��׵�ַ) len(���ݵĳ���)
 *return  ��д�����ݵĳ���
 ****************************************************************/
 int usart1_data_fifo_write(uint8_t* usart1_datas, int len)
{
	int i;

	for(i = 0; i < len; i++){
		if(usart1_data_fifo_put(usart1_datas[i]) == -1)
			break;
	}
	return i;
}

/****************************************************************
 *Function: int usart1_data_fifo_read(uint8_t* usart1_datas, int len)
 *Describe: �����ݻ���������len�ֽ�����
 *Input   ��usart1_data(���ݴ洢���׵�ַ) len(���ݵĳ���)
 *return  ���������ݵĳ���
 ****************************************************************/
int usart1_data_fifo_read(uint8_t* usart1_datas, int len)
{
	int i;

	for(i = 0; i < len; i++){
		if(usart1_data_fifo_get(usart1_datas+i) == -1)
			break;
	}
	
	return i;
}

