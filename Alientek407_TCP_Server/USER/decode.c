#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"

//����������ݵ����ݻ����С
//#define TIMXX_LWIPSENDBUF_LEN 2000
/*�������ݰ���ѡ���*/

char rqst_buf[] = "sRNLMDscandata";//������������ݰ���������'\0',����14
//char *recv_buf; //��SPI���յ������ݰ��Ĵ洢����ַ��ָ��
//char *send_buf; //���������ݰ��Ĵ洢����ַ��ָ��
unsigned int TIMXX_Decode(char *send_buf, char *recv_buf)
{
	uint16_t i;
	unsigned int pstr;	//ָ��������ݰ��ַ���'\0'��ƫ����
    /*�������ݰ�*/
	pstr = 0;
  //pstr += sprintf(&send_buf[pstr],"%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n\r",recv_buf[0],recv_buf[1],recv_buf[2],recv_buf[3],recv_buf[4],recv_buf[5],recv_buf[6],recv_buf[7],recv_buf[8],recv_buf[9],recv_buf[10],recv_buf[11],recv_buf[12],recv_buf[13]);
  pstr += sprintf(&send_buf[pstr],"version number: %d\n\r", *(uint16_t *)&recv_buf[TIMXX_VERSION_NUMBER]);
	/*device information*/
	pstr += sprintf(&send_buf[pstr],"device number: %d\n\r", *(uint16_t *)&recv_buf[TIMXX_DEVICE_NUMBER]);
	pstr += sprintf(&send_buf[pstr],"serial number: %d\n\r", *(uint32_t *)&recv_buf[TIMXX_SERIAL_NUMBER]);
	pstr += sprintf(&send_buf[pstr],"device status: %d\n\r", *(uint16_t *)&recv_buf[TIMXX_DEVICE_STATUS]);
	/*status information*/
	pstr += sprintf(&send_buf[pstr],"telegram counter: %d\n\r", *(uint16_t *)&recv_buf[TIMXX_TELEGRAM_COUNTER]);
	pstr += sprintf(&send_buf[pstr],"scan counter: %d\n\r", *(uint16_t *)&recv_buf[TIMXX_SCAN_COUNTER]);
	pstr += sprintf(&send_buf[pstr],"time since startup: %d\n\r", *(uint32_t *)&recv_buf[TIMXX_TIME_SINCE_STARTUP]);
	pstr += sprintf(&send_buf[pstr],"time of transmission: %d\n\r", *(uint32_t *)&recv_buf[TIMXX_TIME_OF_TRANSMISSION]);
	pstr += sprintf(&send_buf[pstr],"input status: %d\n\r", *(uint16_t *)&recv_buf[TIMXX_INPUT_STATUS]);
	pstr += sprintf(&send_buf[pstr],"output status: %d\n\r", *(uint16_t *)&recv_buf[TIMXX_OUTPUT_STATUS]);
	pstr += sprintf(&send_buf[pstr],"checksum: %d\n\r", *(uint8_t *)&recv_buf[TIMXX_CHECKSUM]);
	/*measerment parameters*/
	pstr += sprintf(&send_buf[pstr],"scanning frequency: %d\n\r", *(uint32_t *)&recv_buf[TIMXX_SCANNING_FREQUENCY]);
	pstr += sprintf(&send_buf[pstr],"measurement frequency: %d\n\r", *(uint32_t *)&recv_buf[TIMXX_MEASUREMENT_FREQUENCY]);
	pstr += sprintf(&send_buf[pstr],"number of 16 bit channels: %d\n\r", *(uint16_t *)&recv_buf[TIMXX_NUMBER_OF_16_BIT_CHANNELS]);
	/*output channels 1*/
	pstr += sprintf(&send_buf[pstr],"%c%c%c%c%c\n\r",recv_buf[TIMXX_MEASURED_DATA_CONTENTS],recv_buf[TIMXX_MEASURED_DATA_CONTENTS+1],recv_buf[TIMXX_MEASURED_DATA_CONTENTS+2],recv_buf[TIMXX_MEASURED_DATA_CONTENTS+3],recv_buf[TIMXX_MEASURED_DATA_CONTENTS+4]);
	pstr += sprintf(&send_buf[pstr],"scaling factor: %f\n\r", *(float *)&recv_buf[TIMXX_SCALING_FACTOR]);
	pstr += sprintf(&send_buf[pstr],"scaling offset: %f\n\r", *(float *)&recv_buf[TIMXX_SCALING_OFFSET]);
	pstr += sprintf(&send_buf[pstr],"starting angle: %d\n\r", *(int32_t *)&recv_buf[TIMXX_STARTING_ANGLE]);
	pstr += sprintf(&send_buf[pstr],"angular step width: %d\n\r", *(uint16_t *)&recv_buf[TIMXX_ANGULAR_STEP_WIDTH]);
	pstr += sprintf(&send_buf[pstr],"number of data: %d\n\r", *(uint16_t *)&recv_buf[TIMXX_NUMBER_OF_DATA]);
	pstr += sprintf(&send_buf[pstr],"data list:\n\r");
	/*91 datas*/
	for(i=0;i<91;i++)
	{
		if(i==90)
			pstr += sprintf(&send_buf[pstr],"%d\n\r",*(uint16_t *)&recv_buf[TIMXX_DATA+i*2]);
		else
			pstr += sprintf(&send_buf[pstr],"%d  ",*(uint16_t *)&recv_buf[TIMXX_DATA+i*2]);
	}
	
	return strlen(send_buf);

	
}