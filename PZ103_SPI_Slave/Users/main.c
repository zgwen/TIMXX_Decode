/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : FLASH-SPI实验
* 实验说明       : 通过将程序下载进去后点击复位，使用芯片内自带的SPI接口
					与FLASH芯片进行通信，通过按键K_UP进行写入和K_DOWN进行写入数据
					的读取，LCD上显示。
					LED指示灯闪烁效果，
					按下复位开关重新运行,其他子函数注释
					可以参考之前的函数注释
* 连接方式       : 	
* 注    意		 : 	所用函数在头文件.c文件内
*******************************************************************************/

#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "led.h"
#include "system.h"
#include "spi.h"
#include "decode.h"

/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/


int main(void)
{
  int i,k;
	char SPI_Sendbuf[TIMXX_RECV_LEN];	//SPI2发送缓冲区
	
	LED_Config();	
	/*
	SPI2_NSS --PB12
	SPI2_SCK --PB13
	SPI2_MISO --PB14
	SPI2_MOSI --PB15
	*/
	SPI2_Config();
	
	//初始化内存
	memset(SPI_Sendbuf, 0, TIMXX_RECV_LEN);
	//构造数据包内容
	sprintf(&SPI_Sendbuf[TIMXX_TYPE_OF_COMMAND], "sSNLMDscandata");
	*(uint16_t *)&SPI_Sendbuf[TIMXX_VERSION_NUMBER] = 222;
	/*device information*/
    *(uint16_t *)&SPI_Sendbuf[TIMXX_DEVICE_NUMBER] = 1995;
	*(uint32_t *)&SPI_Sendbuf[TIMXX_SERIAL_NUMBER] = 31632891;
	*(uint16_t *)&SPI_Sendbuf[TIMXX_DEVICE_STATUS] = 1;
	/*status information*/
	*(uint16_t *)&SPI_Sendbuf[TIMXX_TELEGRAM_COUNTER] = 250;
	*(uint16_t *)&SPI_Sendbuf[TIMXX_SCAN_COUNTER] = 251;
	*(uint32_t *)&SPI_Sendbuf[TIMXX_TIME_SINCE_STARTUP] = 1234567;
	*(uint32_t *)&SPI_Sendbuf[TIMXX_TIME_OF_TRANSMISSION] = 7654321;
	*(uint16_t *)&SPI_Sendbuf[TIMXX_INPUT_STATUS] = 254;
	*(uint16_t *)&SPI_Sendbuf[TIMXX_OUTPUT_STATUS] = 255;
	*(uint16_t *)&SPI_Sendbuf[TIMXX_CHECKSUM] = 26;
	/*measerment parameters*/
	*(uint32_t *)&SPI_Sendbuf[TIMXX_SCANNING_FREQUENCY] = 1500;
	*(uint32_t *)&SPI_Sendbuf[TIMXX_MEASUREMENT_FREQUENCY] = 234123;
	*(uint16_t *)&SPI_Sendbuf[TIMXX_NUMBER_OF_16_BIT_CHANNELS] = 1;
	/*output channels 1*/
	sprintf(&SPI_Sendbuf[TIMXX_MEASURED_DATA_CONTENTS], "DIST1");
	*(float *)&SPI_Sendbuf[TIMXX_SCALING_FACTOR] = 12.34567;
	*(float *)&SPI_Sendbuf[TIMXX_SCALING_OFFSET] = 76.54321;
	*(int32_t *)&SPI_Sendbuf[TIMXX_STARTING_ANGLE] = -900000;
	*(uint16_t *)&SPI_Sendbuf[TIMXX_ANGULAR_STEP_WIDTH] = 30000;
	*(uint16_t *)&SPI_Sendbuf[TIMXX_NUMBER_OF_DATA] = 91;
	/*91 datas*/
	for(i = 0; i < 91; i++)
	{
		*(uint16_t *)&SPI_Sendbuf[TIMXX_DATA+i*2]=i+1000;
	}

	
	while(1)
	{
			//request验证
			if(SPI2_WriteReadData(0x00)=='s')
				if(SPI2_WriteReadData(0x00)=='R')
					if(SPI2_WriteReadData(0x00)=='N')
					{
							k=0;
							while(k<10)
							{
								SPI2_WriteReadData(0x00);
								k++;
							}
							if(SPI2_WriteReadData(0x00)=='a')
							{
								k=0;
								//将SPI_Sendbuf发送出去
								while(k < TIMXX_RECV_LEN)	//循环发送SPI_Sendbuf，
								{
									SPI2_WriteReadData(SPI_Sendbuf[k++]);	//无超时发送，等待主机读取
								}
								PCout(0) = ~PCout(0);	//LED指示工作状态
							}
					}
			
		}
}

