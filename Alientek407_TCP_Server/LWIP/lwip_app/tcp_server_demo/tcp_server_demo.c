#include "tcp_server_demo.h"
#include "lwip/opt.h"
#include "lwip_comm.h"
#include "led.h"
#include "lwip/lwip_sys.h"
#include "lwip/api.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32F407¿ª·¢°å
//NETCONN API±à³Ì·½Ê½µÄTCP·şÎñÆ÷²âÊÔ´úÂë	   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//´´½¨ÈÕÆÚ:2014/8/15
//°æ±¾£ºV1.0
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//ĞŞ¸ÄĞÅÏ¢
//ÎŞ
////////////////////////////////////////////////////////////////////////////////// 	   
 

u8 tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];	//TCP¿Í»§¶Ë½ÓÊÕÊı¾İ»º³åÇø
u8 tcp_server_sendbuf[TCP_SERVER_TX_BUFSIZE];	//TCP¿Í»§¶Ë·¢ËÍÊı¾İ»º³åÇø
u8 tcp_server_flag;								//TCP·şÎñÆ÷Êı¾İ·¢ËÍ±êÖ¾Î»

//TCP¿Í»§¶ËÈÎÎñ
#define TCPSERVER_PRIO		6
//ÈÎÎñ¶ÑÕ»´óĞ¡
#define TCPSERVER_STK_SIZE	300
//ÈÎÎñ¶ÑÕ»
OS_STK TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE];

//tcp·şÎñÆ÷ÈÎÎñ
static void tcp_server_thread(void *arg)
{
	OS_CPU_SR cpu_sr;
	u32 data_len = 0;
	struct pbuf *q;
	err_t err,recv_err;
	u8 remot_addr[4];
	struct netconn *conn, *newconn;
	static ip_addr_t ipaddr;
	static u16_t 			port;
	
	LWIP_UNUSED_ARG(arg);

	conn = netconn_new(NETCONN_TCP);  //´´½¨Ò»¸öTCPÁ´½Ó
	netconn_bind(conn,IP_ADDR_ANY,TCP_SERVER_PORT);  //°ó¶¨¶Ë¿ÚºÅÚ
	netconn_listen(conn);  		//½øÈë¼àÌıÄ£Ê½
	conn->recv_timeout = 10;  	//½ûÖ¹×èÈûÏß³Ì µÈ´ı10ms
	while (1) 
	{
		err = netconn_accept(conn,&newconn);  //½ÓÊÕÁ¬½ÓÇëÇó
		if(err==ERR_OK)newconn->recv_timeout = 10;

		if (err == ERR_OK)    //´¦ÀíĞÂÁ¬½ÓµÄÊı¾İ
		{ 
			struct netbuf *recvbuf;

			netconn_getaddr(newconn,&ipaddr,&port,0); //»ñÈ¡Ô¶¶ËIPµØÖ·ºÍ¶Ë¿ÚºÅ
			
			remot_addr[3] = (uint8_t)(ipaddr.addr >> 24); 
			remot_addr[2] = (uint8_t)(ipaddr.addr>> 16);
			remot_addr[1] = (uint8_t)(ipaddr.addr >> 8);
			remot_addr[0] = (uint8_t)(ipaddr.addr);
			printf("Ö÷»ú%d.%d.%d.%dÁ¬½ÓÉÏ·şÎñÆ÷,Ö÷»ú¶Ë¿ÚºÅÎª:%d\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3],port);
			
			while(1)
			{
				if((tcp_server_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //ÓĞÊı¾İÒª·¢ËÍ
				{
					err = netconn_write(newconn ,tcp_server_sendbuf,strlen((char*)tcp_server_sendbuf),NETCONN_COPY); //·¢ËÍtcp_server_sendbufÖĞµÄÊı¾İ
					if(err != ERR_OK)
					{
						printf("·¢ËÍÊ§°Ü\r\n");
					}
					tcp_server_flag &= ~LWIP_SEND_DATA;
				}
				
				if((recv_err = netconn_recv(newconn,&recvbuf)) == ERR_OK)  	//½ÓÊÕµ½Êı¾İ
				{		
					OS_ENTER_CRITICAL(); //¹ØÖĞ¶Ï
					memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //Êı¾İ½ÓÊÕ»º³åÇøÇåÁã
					for(q=recvbuf->p;q!=NULL;q=q->next)  //±éÀúÍêÕû¸öpbufÁ´±í
					{
						//ÅĞ¶ÏÒª¿½±´µ½TCP_SERVER_RX_BUFSIZEÖĞµÄÊı¾İÊÇ·ñ´óÓÚTCP_SERVER_RX_BUFSIZEµÄÊ£Óà¿Õ¼ä£¬Èç¹û´óÓÚ
						//µÄ»°¾ÍÖ»¿½±´TCP_SERVER_RX_BUFSIZEÖĞÊ£Óà³¤¶ÈµÄÊı¾İ£¬·ñÔòµÄ»°¾Í¿½±´ËùÓĞµÄÊı¾İ
						if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//¿½±´Êı¾İ
						else memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_SERVER_RX_BUFSIZE) break; //³¬³öTCP¿Í»§¶Ë½ÓÊÕÊı×é,Ìø³ö	
					}
					OS_EXIT_CRITICAL();  //¿ªÖĞ¶Ï
					data_len=0;  //¸´ÖÆÍê³Éºódata_lenÒªÇåÁã¡£	
					
					printf("%s\r\n",tcp_server_recvbuf);  //Í¨¹ı´®¿Ú·¢ËÍ½ÓÊÕµ½µÄÊı¾İ
					//½«ÊÕµ½µÄÊı¾İ·¢Ò»·İ¸ø¿Í»§¶Ë
					OS_ENTER_CRITICAL(); //¹ØÖĞ¶Ï
					memset(tcp_server_sendbuf,0,TCP_SERVER_TX_BUFSIZE);  //·¢ËÍ½ÓÊÕ»º³åÇøÇåÁã
					memcpy(tcp_server_sendbuf,tcp_server_recvbuf,strlen((char*)tcp_server_recvbuf)); //¿½±´Êı¾İ
					tcp_server_flag |= LWIP_SEND_DATA;
					OS_EXIT_CRITICAL();  //¿ªÖĞ¶Ï
					
					netbuf_delete(recvbuf);
				}else if(recv_err == ERR_CLSD)  //¹Ø±ÕÁ¬½Ó
				{
					netconn_close(newconn);
					netconn_delete(newconn);
					printf("Ö÷»ú:%d.%d.%d.%d¶Ï¿ªÓë·şÎñÆ÷µÄÁ¬½Ó\r\n",remot_addr[0], remot_addr[1],remot_addr[2],remot_addr[3]);
					break;
				}
			}
		}
	}
}


//´´½¨TCP·şÎñÆ÷Ïß³Ì
//·µ»ØÖµ:0 TCP·şÎñÆ÷´´½¨³É¹¦
//		ÆäËû TCP·şÎñÆ÷´´½¨Ê§°Ü
INT8U tcp_server_init(void)
{
	INT8U res;
	OS_CPU_SR cpu_sr;
	
	OS_ENTER_CRITICAL();	//¹ØÖĞ¶Ï
	res = OSTaskCreate(tcp_server_thread,(void*)0,(OS_STK*)&TCPSERVER_TASK_STK[TCPSERVER_STK_SIZE-1],TCPSERVER_PRIO); //´´½¨TCP·şÎñÆ÷Ïß³Ì
	OS_EXIT_CRITICAL();		//¿ªÖĞ¶Ï
	
	return res;
}


