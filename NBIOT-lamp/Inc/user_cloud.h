#ifndef _USER_CLOUD_H_
#define _USER_CLOUD_H_
#include "stm32l1xx_hal.h"

#define CDP_SERVER_IP		"117.60.157.137"

#define CDP_SERVER_PORT		5683

//Define PDP Context
#define PDP_CID			1
#define PDP_TYPE	"IP"
#define PDP_APN		"ctnet"		

//Change device functionality (+CFUN_ON)
#define	CFUN_ON		1  //full functionality
#define	CFUN_OFF	0

//Signalling connection status (+CSCON)
#define	CSCON		0  //disable unsolicited result code


//EPS network registration status (+CEREG)
#define	CEREG		2 //enable network registration and location information unsolicited result code +CEREG: <stat>[,[<tac>],[<ci>],[<AcT>]]

//Neul New Message Indications (+NNMI)
#define NNMI		1//Indications and Message


//PS attach or detach (+CGATT)
#define CGATT		1


#define LINK_OK	1
#define TIME_OK	2
#define RCV_OK  3
#define CONTROL_OPEN 4
#define CONTROL_CLOSE	5



typedef struct{
	unsigned char batteryLevel;
	short RSRP;
	char ECL;
	char SNR;
}BNIOT_DEVICE_INF;

void wait_nbiot_start(void);
void nbiot_config(void);
void link_server(void);
void get_time_from_server(void);
uint8_t rcv_data_deal(void);
void send_data_to_cloud(int illumination,uint8_t light_status);


#endif //_USER_CLOUD_H_
