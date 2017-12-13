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

//ALIENTEK ̽����STM32F407������
//����NETCONN API��TCP������ʵ��
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
	char *spi_recvbuf;		//SPI�������ݻ���
//SPI����
#define SPI_TASK_PRIO 		8
//�����ջ��С
#define SPI_STK_SIZE		128	
//�����ջ
OS_STK SPI_TASK_STK[SPI_STK_SIZE];
//������
void spi_task(void *pdata);   

//LED����
//�������ȼ�
#define LED_TASK_PRIO		9
//�����ջ��С
#define LED_STK_SIZE		64
//�����ջ
OS_STK	LED_TASK_STK[LED_STK_SIZE];
//������
void led_task(void *pdata);  

//�ڴ�������ʾ��ַ��Ϣ����
//�������ȼ�
#define DISPLAY_TASK_PRIO	10
//�����ջ��С
#define DISPLAY_STK_SIZE	128
//�����ջ
OS_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//������
void display_task(void *pdata);

//START����
//�������ȼ�
#define START_TASK_PRIO		11
//�����ջ��С
#define START_STK_SIZE		128
//�����ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata); 

//��LCD����ʾ��ַ��Ϣ
//mode:1 ��ʾDHCP��ȡ���ĵ�ַ
//	  ���� ��ʾ��̬��ַ
void show_address(u8 mode)
{
	u8 buf[30];
	if(mode==2)
	{
		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d\r\n",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//��ӡ��̬IP��ַ
		printf((char*)buf);
		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d\r\n",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//��ӡ���ص�ַ
		printf((char*)buf);
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d\r\n",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//��ӡ���������ַ
		printf((char*)buf);
		printf("Port:%4d!\r\n",TCP_SERVER_PORT);
	}
	else 
	{
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d\r\n",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//��ӡ��̬IP��ַ
		printf((char*)buf);
		sprintf((char*)buf,"Static GW:%d.%d.%d.%d\r\n",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//��ӡ���ص�ַ
		printf((char*)buf);
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d\r\n",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//��ӡ���������ַ
		printf((char*)buf);
		printf("Port:%4d!\r\n",TCP_SERVER_PORT);
	}	
}

int main(void)
{
	delay_init(168);       	//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϷ�������
	uart_init(115200);   	//���ڲ���������
	usmart_dev.init(84);	//��ʼ��USMART
	LED_Init(); 			//LED��ʼ��
	SPI2_Init();	//SPI2��ʼ��
	
	
	FSMC_SRAM_Init(); 		//SRAM��ʼ��
	
	mymem_init(SRAMIN);  	//��ʼ���ڲ��ڴ��
	mymem_init(SRAMEX);  	//��ʼ���ⲿ�ڴ��
	mymem_init(SRAMCCM); 	//��ʼ��CCM�ڴ��
	spi_recvbuf = (char *)mymalloc(SRAMIN,TIMXX_RECV_LEN);
	printf("TCP Server NETCONN Test\r\n");
	printf("KEY0:Send data\r\n");
	printf("date: 2017/12/09\r\n");

	OSInit(); 					//UCOS��ʼ��
	while(lwip_comm_init()) 	//lwip��ʼ��
	{
		printf("Lwip Init failed!\r\n");	//lwip��ʼ��ʧ��
		
		delay_ms(1000);
		
	}
	printf("Lwip Init Success!\r\n"); 		//lwip��ʼ���ɹ�
	while(tcp_server_init()) 									//��ʼ��tcp_server(����tcp_server�߳�)
	{
		printf("TCP Server failed!\r\n"); //tcp����������ʧ��
		delay_ms(1000);
	}
	printf("TCP Server Success!\r\n"); 	//tcp�����������ɹ�
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
	OSStart(); //����UCOS
}

//start����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	
	OSStatInit();  			//��ʼ��ͳ������
	OS_ENTER_CRITICAL();  	//���ж�
#if	LWIP_DHCP
	lwip_comm_dhcp_creat();	//����DHCP����
#endif
	OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO); 	//����LED����
	OSTaskCreate(spi_task,(void*)0,(OS_STK*)&SPI_TASK_STK[SPI_STK_SIZE-1],SPI_TASK_PRIO); 	//����SPI����
	OSTaskCreate(display_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO); //��ʾ����
	OSTaskSuspend(OS_PRIO_SELF); //����start_task����
	OS_EXIT_CRITICAL();  //���ж�
}

//��ʾ��ַ����Ϣ��������
void display_task(void *pdata)
{
	while(1)
	{ 
#if LWIP_DHCP									//������DHCP��ʱ��
		if(lwipdev.dhcpstatus != 0) 			//����DHCP
		{
			show_address(lwipdev.dhcpstatus );	//��ʾ��ַ��Ϣ
			OSTaskSuspend(OS_PRIO_SELF); 		//��ʾ���ַ��Ϣ�������������
		}
#else
		show_address(0); 						//��ʾ��̬��ַ
		OSTaskSuspend(OS_PRIO_SELF); 			//��ʾ���ַ��Ϣ�������������
#endif //LWIP_DHCP
		OSTimeDlyHMSM(0,0,0,500);
	}
}

//spi����
extern u8 tcp_server_sendbuf[TCP_SERVER_TX_BUFSIZE];	//TCP�ͻ��˷������ݻ�����
extern u8 tcp_server_flag;
void spi_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	u8 recv_ok=0;
	unsigned int i;

	while(1)
	{
		//���spi_recvbuf�Ƿ���д
		if(!recv_ok)
		{
			//����request
			for(i=0;i<14;i++)
			{
			SPI2_ReadWriteByte(rqst_buf[i]);
			}
			//�����յ������ݿ�ͷ����������Ǻϣ����������İ�
			if(SPI2_ReadWriteByte(0x00)=='s')
				if(SPI2_ReadWriteByte(0x00)=='S')
					if(SPI2_ReadWriteByte(0x00)=='N')
					{
						OS_ENTER_CRITICAL(); //���ж�
						memset(spi_recvbuf,0,TIMXX_RECV_LEN);//���ս��ջ���������
						i = 0;
						spi_recvbuf[i++] = 's';
						spi_recvbuf[i++] = 'S';
						spi_recvbuf[i++] = 'N';
						//����ʣ������
						while(i < TIMXX_RECV_LEN)
						{
							(spi_recvbuf[i++]= SPI2_ReadWriteByte(0x00));
						}
						recv_ok = 1;	//��־spi�������
						OS_EXIT_CRITICAL();  //���ж�
					}
			}
		//�ж�spi�Ƿ������������Լ�LWIP���ͻ����Ƿ��
		if(recv_ok && (!(tcp_server_flag & LWIP_SEND_DATA)))
		{
				recv_ok = 0;	//��־���������
				OS_ENTER_CRITICAL(); 
				memset(tcp_server_sendbuf,0,TCP_SERVER_TX_BUFSIZE);  //LWIP���ͻ���������
				i=TIMXX_Decode((char *)tcp_server_sendbuf,(char *)spi_recvbuf);	//���룬��������д��tcp_server_sendbuf
				printf("String len: %d\r\n",i);
				tcp_server_flag |= LWIP_SEND_DATA;	//֪ͨLWIP���ݴ���
				OS_EXIT_CRITICAL(); 
		}
		OSTimeDlyHMSM(0,0,0,500);  //��ʱ50ms
	}
}

//led����
void led_task(void *pdata)
{
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500);  //��ʱ500ms
 	}
}










