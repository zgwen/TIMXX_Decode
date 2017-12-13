#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "spi.h"
#include "lwip_comm.h"
#include "LAN8720.h"
#include "usmart.h"
#include "timer.h"
#include "sram.h"
#include "malloc.h"
#include "lwip_comm.h"
#include "includes.h"
#include "lwipopts.h"
#include "tcp_server_demo.h"
#include "decode.h"

//ALIENTEK 探索者STM32F407开发板
//基于NETCONN API的TCP服务器实验
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
	char *spi_recvbuf;		//SPI接收数据缓冲
//SPI任务
#define SPI_TASK_PRIO 		8
//任务堆栈大小
#define SPI_STK_SIZE		128	
//任务堆栈
OS_STK SPI_TASK_STK[SPI_STK_SIZE];
//任务函数
void spi_task(void *pdata);   

//LED任务
//任务优先级
#define LED_TASK_PRIO		9
//任务堆栈大小
#define LED_STK_SIZE		64
//任务堆栈
OS_STK	LED_TASK_STK[LED_STK_SIZE];
//任务函数
void led_task(void *pdata);  

//在串口上显示地址信息任务
//任务优先级
#define DISPLAY_TASK_PRIO	10
//任务堆栈大小
#define DISPLAY_STK_SIZE	128
//任务堆栈
OS_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//任务函数
void display_task(void *pdata);

//START任务
//任务优先级
#define START_TASK_PRIO		11
//任务堆栈大小
#define START_STK_SIZE		128
//任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata); 

//在LCD上显示地址信息
//mode:1 显示DHCP获取到的地址
//	  其他 显示静态地址
void show_address(u8 mode)
{
	u8 buf[30];
	if(mode==2)
	{
		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d\r\n",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		printf((char*)buf);
		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d\r\n",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		printf((char*)buf);
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d\r\n",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		printf((char*)buf);
		printf("Port:%4d!\r\n",TCP_SERVER_PORT);
	}
	else 
	{
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d\r\n",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		printf((char*)buf);
		sprintf((char*)buf,"Static GW:%d.%d.%d.%d\r\n",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		printf((char*)buf);
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d\r\n",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		printf((char*)buf);
		printf("Port:%4d!\r\n",TCP_SERVER_PORT);
	}	
}

int main(void)
{
	delay_init(168);       	//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组配置
	uart_init(115200);   	//串口波特率设置
	usmart_dev.init(84);	//初始化USMART
	LED_Init(); 			//LED初始化
	SPI2_Init();	//SPI2初始化
	
	
	FSMC_SRAM_Init(); 		//SRAM初始化
	
	mymem_init(SRAMIN);  	//初始化内部内存池
	mymem_init(SRAMEX);  	//初始化外部内存池
	mymem_init(SRAMCCM); 	//初始化CCM内存池
	spi_recvbuf = (char *)mymalloc(SRAMIN,TIMXX_RECV_LEN);
	printf("TCP Server NETCONN Test\r\n");
	printf("KEY0:Send data\r\n");
	printf("date: 2017/12/09\r\n");

	OSInit(); 					//UCOS初始化
	while(lwip_comm_init()) 	//lwip初始化
	{
		printf("Lwip Init failed!\r\n");	//lwip初始化失败
		
		delay_ms(1000);
		
	}
	printf("Lwip Init Success!\r\n"); 		//lwip初始化成功
	while(tcp_server_init()) 									//初始化tcp_server(创建tcp_server线程)
	{
		printf("TCP Server failed!\r\n"); //tcp服务器创建失败
		delay_ms(1000);
	}
	printf("TCP Server Success!\r\n"); 	//tcp服务器创建成功
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
	OSStart(); //开启UCOS
}

//start任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	
	OSStatInit();  			//初始化统计任务
	OS_ENTER_CRITICAL();  	//关中断
#if	LWIP_DHCP
	lwip_comm_dhcp_creat();	//创建DHCP任务
#endif
	OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO); 	//创建LED任务
	OSTaskCreate(spi_task,(void*)0,(OS_STK*)&SPI_TASK_STK[SPI_STK_SIZE-1],SPI_TASK_PRIO); 	//创建SPI任务
	OSTaskCreate(display_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO); //显示任务
	OSTaskSuspend(OS_PRIO_SELF); //挂起start_task任务
	OS_EXIT_CRITICAL();  //开中断
}

//显示地址等信息的任务函数
void display_task(void *pdata)
{
	while(1)
	{ 
#if LWIP_DHCP									//当开启DHCP的时候
		if(lwipdev.dhcpstatus != 0) 			//开启DHCP
		{
			show_address(lwipdev.dhcpstatus );	//显示地址信息
			OSTaskSuspend(OS_PRIO_SELF); 		//显示完地址信息后挂起自身任务
		}
#else
		show_address(0); 						//显示静态地址
		OSTaskSuspend(OS_PRIO_SELF); 			//显示完地址信息后挂起自身任务
#endif //LWIP_DHCP
		OSTimeDlyHMSM(0,0,0,500);
	}
}

//spi任务
extern u8 tcp_server_sendbuf[TCP_SERVER_TX_BUFSIZE];	//TCP客户端发送数据缓冲区
extern u8 tcp_server_flag;
void spi_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	u8 recv_ok=0;
	unsigned int i;

	while(1)
	{
		//检测spi_recvbuf是否需写
		if(!recv_ok)
		{
			//发送request
			for(i=0;i<14;i++)
			{
			SPI2_ReadWriteByte(rqst_buf[i]);
			}
			//检验收到的数据开头，如果数据吻合，接收完整的包
			if(SPI2_ReadWriteByte(0x00)=='s')
				if(SPI2_ReadWriteByte(0x00)=='S')
					if(SPI2_ReadWriteByte(0x00)=='N')
					{
						OS_ENTER_CRITICAL(); //关中断
						memset(spi_recvbuf,0,TIMXX_RECV_LEN);//接收接收缓冲区清零
						i = 0;
						spi_recvbuf[i++] = 's';
						spi_recvbuf[i++] = 'S';
						spi_recvbuf[i++] = 'N';
						//接收剩余数据
						while(i < TIMXX_RECV_LEN)
						{
							(spi_recvbuf[i++]= SPI2_ReadWriteByte(0x00));
						}
						recv_ok = 1;	//标志spi接收完成
						OS_EXIT_CRITICAL();  //开中断
					}
			}
		//判断spi是否接收数据完成以及LWIP发送缓冲是否空
		if(recv_ok && (!(tcp_server_flag & LWIP_SEND_DATA)))
		{
				recv_ok = 0;	//标志需接收数据
				OS_ENTER_CRITICAL(); 
				memset(tcp_server_sendbuf,0,TCP_SERVER_TX_BUFSIZE);  //LWIP发送缓冲区清零
				i=TIMXX_Decode((char *)tcp_server_sendbuf,(char *)spi_recvbuf);	//译码，并将数据写入tcp_server_sendbuf
				printf("String len: %d\r\n",i);
				tcp_server_flag |= LWIP_SEND_DATA;	//通知LWIP数据待发
				OS_EXIT_CRITICAL(); 
		}
		OSTimeDlyHMSM(0,0,0,500);  //延时50ms
	}
}

//led任务
void led_task(void *pdata)
{
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500);  //延时500ms
 	}
}










