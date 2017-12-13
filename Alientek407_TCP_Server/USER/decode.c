#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"
#include "decode.h"

//����������ݵ����ݻ����С
//#define TIMXX_LWIPSENDBUF_LEN 2000
/*�������ݰ���ѡ���*/

char rqst_buf[] = "sRNLMDscandata";//������������ݰ���������'\0',����14
//char *recv_buf; //��SPI���յ������ݰ��Ĵ洢����ַ��ָ��
//char *send_buf; //���������ݰ��Ĵ洢����ַ��ָ��
unsigned int TIMXX_Decode(char *send_buf,  struct TIMXXData *recv_buf)
{
	uint16_t i;
	unsigned int pstr;	//ָ��������ݰ��ַ���'\0'��ƫ����
	char *recv_type;
	char *recv_cont;
	//�����ڴ�
	recv_type = (char *)mymalloc(SRAMIN,15);
	recv_cont = (char *)mymalloc(SRAMIN,6);
   /*�������ݰ�*/
	pstr = 0;
	#ifdef TIMXX_COMMAND
	memcpy(recv_type,recv_buf->typeofcmd,3);
	memcpy(&recv_type[3],recv_buf->cmd,11);
	recv_type[14] = '\0';
  pstr += sprintf(&send_buf[pstr],"%s\n\r",recv_type);
	#endif
	#ifdef TIMXX_VERSION_NUMBER
  pstr += sprintf(&send_buf[pstr],"version number: %d\n\r", recv_buf->versionnum);
	#endif
	/*device information*/
	#ifdef TIMXX_DEVICE_NUMBER
	pstr += sprintf(&send_buf[pstr],"device number: %d\n\r", recv_buf->devicenum);
	#endif
	#ifdef TIMXX_SERIAL_NUMBER
	pstr += sprintf(&send_buf[pstr],"serial number: %d\n\r", recv_buf->serialnum);
	#endif
	#ifdef TIMXX_DEVICE_STATUS
	pstr += sprintf(&send_buf[pstr],"device status: %d\n\r", recv_buf->devicestat);
	/*status information*/
	#endif
	#ifdef TIMXX_TELEGRAM_COUNTER
	pstr += sprintf(&send_buf[pstr],"telegram counter: %d\n\r", recv_buf->telegramcount);
	#endif
	#ifdef TIMXX_SCAN_COUNTER
	pstr += sprintf(&send_buf[pstr],"scan counter: %d\n\r", recv_buf->scancount);
	#endif
	#ifdef TIMXX_TIME_SINCE_STARTUP
	pstr += sprintf(&send_buf[pstr],"time since startup: %d\n\r", recv_buf->timesincestart);
	#endif
	#ifdef TIMXX_TIME_OF_TRANSMISSION
	pstr += sprintf(&send_buf[pstr],"time of transmission: %d\n\r",recv_buf->timeoftrans);
	#endif
	#ifdef TIMXX_INPUT_STATUS
	pstr += sprintf(&send_buf[pstr],"input status: %d\n\r", recv_buf->inputstat);
	#endif
	#ifdef TIMXX_OUTPUT_STATUS
	pstr += sprintf(&send_buf[pstr],"output status: %d\n\r",recv_buf->outputstat);
	#endif
	#ifdef TIMXX_CHECKSUM
	pstr += sprintf(&send_buf[pstr],"checksum: %d\n\r", recv_buf->checksum);
	/*measerment parameters*/
	#endif
	#ifdef TIMXX_SCANNING_FREQUENCY
	pstr += sprintf(&send_buf[pstr],"scanning frequency: %d\n\r", recv_buf->scanfreq);
	#endif
	#ifdef TIMXX_MEASUREMENT_FREQUENCY
	pstr += sprintf(&send_buf[pstr],"measurement frequency: %d\n\r", recv_buf->measurefreq);
	#endif
	#ifdef TIMXX_NUMBER_OF_16_BIT_CHANNELS
	pstr += sprintf(&send_buf[pstr],"number of 16 bit channels: %d\n\r", recv_buf->numof16bitchan);
	/*output channels 1*/
	#endif
	#ifdef TIMXX_MEASURED_DATA_CONTENTS
	memcpy(recv_cont,recv_buf->measuredadacont,5);
	recv_cont[5] = '\0';
  pstr += sprintf(&send_buf[pstr],"%s\n\r",recv_cont);
	#endif
	#ifdef TIMXX_SCALING_FACTOR
	pstr += sprintf(&send_buf[pstr],"scaling factor: %f\n\r", recv_buf->scalfac);
	#endif
	#ifdef TIMXX_SCALING_OFFSET
	pstr += sprintf(&send_buf[pstr],"scaling offset: %f\n\r", recv_buf->scaloffset);
	#endif
	#ifdef TIMXX_STARTING_ANGLE
	pstr += sprintf(&send_buf[pstr],"starting angle: %d\n\r", recv_buf->startangle);
	#endif
	#ifdef TIMXX_ANGULAR_STEP_WIDTH
	pstr += sprintf(&send_buf[pstr],"angular step width: %d\n\r", recv_buf->angstepwidth);
	#endif
	#ifdef TIMXX_NUMBER_OF_DATA
	pstr += sprintf(&send_buf[pstr],"number of data: %d\n\r", recv_buf->numofdata);
	#endif
	#ifdef TIMXX_DATA
	pstr += sprintf(&send_buf[pstr],"data list:\n\r");
	/*91 datas*/
	for(i=0;i<91;i++)
	{
		if(i==90)
			pstr += sprintf(&send_buf[pstr],"%d\n\r",recv_buf->data[i]);
		else
			pstr += sprintf(&send_buf[pstr],"%d  ",recv_buf->data[i]);
	}
	#endif
	myfree(SRAMIN,(void *)recv_type);  //�ͷ��ڴ�
	myfree(SRAMIN,(void *)recv_cont);
	return strlen(send_buf);	//���ؽ������ַ�������
}

