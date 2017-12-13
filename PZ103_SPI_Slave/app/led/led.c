#include "led.h"


/****************************************************************************
* Function Name  : LED_Config
* Description    : 将LED使用的IO口设置为上拉输出
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void LED_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;               //声明一个结构体变量，用来初始化GPIO

    /* 开启GPIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /*  配置GPIO的模式和IO口 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED;        //选择你要设置的IO口
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置传输速率
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置推挽输出模式

    /* 初始化GPIO */
    GPIO_Init(GPIOC, &GPIO_InitStructure);	           //初始化GPIO_LED

    /* 初始化GPIO */
    GPIO_SetBits(GPIOC, GPIO_Pin_LED);                 //将LED的IO口全部赋值为1，即熄灭LED灯
}

/****************************************************************************
* Function Name  : LED_SetState
* Description    : 设置LED的状态，0代表相应的LED点亮，1表示相应的LED熄灭
* Input          : stateValue：LED的状态
* Output         : None
* Return         : None
****************************************************************************/

void LED_SetState(uint8_t stateValue)
{    
    /* 设置LED灯的状态, GPIO一次设置16位，将其值强制转换位16位 */
    GPIO_SetBits(GPIOC, (uint16_t)stateValue & 0x00FF);         //将要熄灭的LED熄灭
    GPIO_ResetBits(GPIOC, (~(uint16_t)stateValue) & 0x00FF);    //将要点亮的LED点亮                  
}



