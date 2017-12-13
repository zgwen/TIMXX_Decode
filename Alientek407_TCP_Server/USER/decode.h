#ifndef _DECODE_H
#define _DECODE_H
#include <stdint.h> 
/*各字段偏移量，输出信息的开关*/
#define TIMXX_TYPE_OF_COMMAND 0 
#define TIMXX_COMMAND 3
#define TIMXX_VERSION_NUMBER 14
/*device information*/
#define TIMXX_DEVICE_NUMBER 16
#define TIMXX_SERIAL_NUMBER 18
#define TIMXX_DEVICE_STATUS 22
/*status information*/
#define TIMXX_TELEGRAM_COUNTER 24
#define TIMXX_SCAN_COUNTER 26
#define TIMXX_TIME_SINCE_STARTUP 28
#define TIMXX_TIME_OF_TRANSMISSION 32
#define TIMXX_INPUT_STATUS 36
#define TIMXX_OUTPUT_STATUS 38
#define TIMXX_CHECKSUM 40
#define TIMXX_RESERVED_BYTE_A 41	//unused
/*measerment parameters*/
#define TIMXX_SCANNING_FREQUENCY 43
#define TIMXX_MEASUREMENT_FREQUENCY 47
//const 0
#define TIMXX_NUMBER_OF_ENCODERS 51	//unused
//const 1
#define TIMXX_NUMBER_OF_16_BIT_CHANNELS 53
/*output channels 1*/
//#define TIMXX_MEASURED_DATA_CONTENTS 55 
#define TIMXX_SCALING_FACTOR 60
#define TIMXX_SCALING_OFFSET 64
#define TIMXX_STARTING_ANGLE 68
#define TIMXX_ANGULAR_STEP_WIDTH 72 
//const 91
#define TIMXX_NUMBER_OF_DATA 74
#define TIMXX_DATA 76

//reserved, unused
#define TIMXX_NUM_OF_8_BIT_CHANNELS 258
#define TIMXX_POSITION 260
#define TIMXX_NAME 262
#define TIMXX_COMMENT 264
#define TIMXX_TIME_INFORMATION 266
#define TIMXX_EVENT_INFORMATION 268


/*定义数据包结构体*/
#pragma pack(1)  //关闭字节对齐
struct TIMXXData{
 char typeofcmd[3];
 char cmd[11];
 uint16_t versionnum;
 uint16_t devicenum;
 uint32_t serialnum;
 uint16_t devicestat;
 uint16_t telegramcount;
 uint16_t scancount;
 uint32_t timesincestart;
 uint32_t timeoftrans;
 uint16_t inputstat;
 uint16_t outputstat;
 uint8_t checksum;
 uint16_t resbytea;
 uint32_t scanfreq;
 uint32_t measurefreq;
 uint16_t numofencod;
 uint16_t numof16bitchan;
 char measuredadacont[5];
 float scalfac;
 float scaloffset;
 int32_t startangle;
 uint16_t angstepwidth;
 uint16_t numofdata;
 uint16_t data[91];
 uint16_t numof8bitchan;
 uint16_t position;
 uint16_t name;
 uint16_t comment;
 uint16_t timeinfo;
 uint16_t eventinfo;
};
#pragma pack()   //打开字节对齐

#define TIMXX_RECV_LEN  sizeof(struct TIMXXData)

extern char rqst_buf[];
unsigned int TIMXX_Decode(char *send_buf, struct TIMXXData *recv_buf);

#endif

