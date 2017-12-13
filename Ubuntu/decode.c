#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"

//解析后的数据的数据缓冲大小
#define LWIP_SENDBUF_LEN 2000
/*解析数据包的选项开关*/

char rqst_buf[] = "sRNLMDscandata";//发送请求的数据包，不包括'0',长度14
char timtypeofcmd[] = "sSN";
char timcmd[] = "LMDscandata";
char cont_buf[] = "DIST1";
struct TIMXXData *recv_buf; //构造的数据包的存储区基址的指针
char *send_buf; //解析的数据包的存储区基址的指针

int main(int argc, char *argv)
{
	uint16_t i;
	uint16_t pstr;
	char *recv_type;
	char *recv_cont;
	/*申请内存*/
	recv_type = (char *)malloc(15);
	recv_cont = (char *)malloc(6);
    recv_buf = (struct TIMXXData *)malloc(sizeof(struct TIMXXData)); //接收数据的数据包
    send_buf = (char *)malloc(LWIP_SENDBUF_LEN); //解析数据的数据包
    /*重置内存*/
    memset(recv_buf, 0, TIMXX_RECV_LEN);
    //构造数据包内容
    memcpy(recv_buf->typeofcmd,timtypeofcmd,3);
	memcpy(recv_buf->cmd,timcmd,11);
    recv_buf->versionnum = 222;
	/*device information*/
    recv_buf->devicenum = 1995;
	recv_buf->serialnum= 31632891;
	recv_buf->devicestat = 1;
	/*status information*/
	recv_buf->telegramcount = 250;
	recv_buf->scancount = 251;
	recv_buf->timesincestart = 1234567;
	recv_buf->timeoftrans = 7654321;
	recv_buf->inputstat = 254;
	recv_buf->outputstat = 255;
	recv_buf->checksum = 26;
	recv_buf->resbytea = 0;
	/*measerment parameters*/
	recv_buf->scanfreq = 1500;
	recv_buf->measurefreq = 234123;
	recv_buf->numofencod = 0;
	recv_buf->numof16bitchan = 1;
	/*output channels 1*/
	memcpy(recv_buf->measuredadacont,cont_buf,5);
	recv_buf->scalfac = 12.34567;
	recv_buf->scaloffset = 76.54321;
	recv_buf->startangle = -900000;
	recv_buf->angstepwidth = 30000;
	recv_buf->numofdata = 91;
	/*91 datas*/
	for(i = 0; i < 91; i++)
	{
		recv_buf->data[i] = i+1000;
	}
	recv_buf->numof8bitchan = 0;
	recv_buf->position = 0;
	recv_buf->name = 0;
	recv_buf->comment = 0;
	recv_buf->timeinfo = 0;
	recv_buf->eventinfo = 0;

	
	/*重置内存*/
	memset(send_buf, 0, LWIP_SENDBUF_LEN);
    /*解析数据包*/
	pstr = 0;
	memcpy(recv_type,recv_buf->typeofcmd,3);
	memcpy(&recv_type[3],recv_buf->cmd,11);
	recv_type[14] = '\0';
    pstr += sprintf(&send_buf[pstr],"%s\n\r",recv_type);
    pstr += sprintf(&send_buf[pstr],"version number: %d\n\r", recv_buf->versionnum);
	/*device information*/
	pstr += sprintf(&send_buf[pstr],"device number: %d\n\r", recv_buf->devicenum);
	pstr += sprintf(&send_buf[pstr],"serial number: %d\n\r", recv_buf->serialnum);
	pstr += sprintf(&send_buf[pstr],"device status: %d\n\r", recv_buf->devicestat);
	/*status information*/
	pstr += sprintf(&send_buf[pstr],"telegram counter: %d\n\r", recv_buf->telegramcount);
	pstr += sprintf(&send_buf[pstr],"scan counter: %d\n\r", recv_buf->scancount);
	pstr += sprintf(&send_buf[pstr],"time since startup: %d\n\r", recv_buf->timesincestart);
	pstr += sprintf(&send_buf[pstr],"time of transmission: %d\n\r",recv_buf->timeoftrans);
	pstr += sprintf(&send_buf[pstr],"input status: %d\n\r", recv_buf->inputstat);
	pstr += sprintf(&send_buf[pstr],"output status: %d\n\r",recv_buf->outputstat);
	pstr += sprintf(&send_buf[pstr],"checksum: %d\n\r", recv_buf->checksum);
	/*measerment parameters*/
	pstr += sprintf(&send_buf[pstr],"scanning frequency: %d\n\r", recv_buf->scanfreq);
	pstr += sprintf(&send_buf[pstr],"measurement frequency: %d\n\r", recv_buf->measurefreq);
	pstr += sprintf(&send_buf[pstr],"number of 16 bit channels: %d\n\r", recv_buf->numof16bitchan);
	/*output channels 1*/
	memcpy(recv_cont,recv_buf->measuredadacont,5);
	recv_cont[5] = '\0';
    pstr += sprintf(&send_buf[pstr],"%s\n\r",recv_cont);
	pstr += sprintf(&send_buf[pstr],"scaling factor: %f\n\r", recv_buf->scalfac);
	pstr += sprintf(&send_buf[pstr],"scaling offset: %f\n\r", recv_buf->scaloffset);
	pstr += sprintf(&send_buf[pstr],"starting angle: %d\n\r", recv_buf->startangle);
	pstr += sprintf(&send_buf[pstr],"angular step width: %d\n\r", recv_buf->angstepwidth);
	pstr += sprintf(&send_buf[pstr],"number of data: %d\n\r", recv_buf->numofdata);
	pstr += sprintf(&send_buf[pstr],"data list:\n\r");
	/*91 datas*/
	for(i=0;i<91;i++)
	{
		if(i==90)
			pstr += sprintf(&send_buf[pstr],"%d\n\r",recv_buf->data[i]);
		else
			pstr += sprintf(&send_buf[pstr],"%d  ",recv_buf->data[i]);
	}

	printf("%s",send_buf);
    printf("str len:%d\r\n",(int)strlen(send_buf));
	printf("TIMXXData size:%d\r\n",(int)sizeof(struct TIMXXData));

	
}
