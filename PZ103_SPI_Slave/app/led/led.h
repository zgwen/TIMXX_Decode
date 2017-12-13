#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

/* 定义要设置的LED使用的PIN口 */
#define GPIO_Pin_LED (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5\
| GPIO_Pin_6 | GPIO_Pin_7)

/* 声明全局函数 */
void LED_Config(void);
void LED_SetState(uint8_t stateValue);















#endif
