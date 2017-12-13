#ifndef __TCP_SERVER_DEMO_H
#define __TCP_SERVER_DEMO_H
#include "sys.h"
#include "includes.h"
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

 
#define TCP_SERVER_RX_BUFSIZE	2000		//¶¨Òåtcp server×î´ó½ÓÊÕÊı¾İ³¤¶È
#define TCP_SERVER_TX_BUFSIZE	2000		//¶¨Òåtcp server×î´ó½ÓÊÕÊı¾İ³¤¶
#define TCP_SERVER_PORT			8088	//¶¨Òåtcp serverµÄ¶Ë¿Ú
#define LWIP_SEND_DATA			0X80	//¶¨ÒåÓĞÊı¾İ·¢ËÍ

extern u8 tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];	//TCP¿Í»§¶Ë½ÓÊÕÊı¾İ»º³åÇø
extern u8 tcp_server_flag;			//TCP·şÎñÆ÷Êı¾İ·¢ËÍ±êÖ¾Î»

INT8U tcp_server_init(void);		//TCP·şÎñÆ÷³õÊ¼»¯(´´½¨TCP·şÎñÆ÷Ïß³Ì)
#endif

