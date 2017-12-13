#include "led.h"


/****************************************************************************
* Function Name  : LED_Config
* Description    : ��LEDʹ�õ�IO������Ϊ�������
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void LED_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;               //����һ���ṹ�������������ʼ��GPIO

    /* ����GPIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /*  ����GPIO��ģʽ��IO�� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED;        //ѡ����Ҫ���õ�IO��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //���ô�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�����������ģʽ

    /* ��ʼ��GPIO */
    GPIO_Init(GPIOC, &GPIO_InitStructure);	           //��ʼ��GPIO_LED

    /* ��ʼ��GPIO */
    GPIO_SetBits(GPIOC, GPIO_Pin_LED);                 //��LED��IO��ȫ����ֵΪ1����Ϩ��LED��
}

/****************************************************************************
* Function Name  : LED_SetState
* Description    : ����LED��״̬��0������Ӧ��LED������1��ʾ��Ӧ��LEDϨ��
* Input          : stateValue��LED��״̬
* Output         : None
* Return         : None
****************************************************************************/

void LED_SetState(uint8_t stateValue)
{    
    /* ����LED�Ƶ�״̬, GPIOһ������16λ������ֵǿ��ת��λ16λ */
    GPIO_SetBits(GPIOC, (uint16_t)stateValue & 0x00FF);         //��ҪϨ���LEDϨ��
    GPIO_ResetBits(GPIOC, (~(uint16_t)stateValue) & 0x00FF);    //��Ҫ������LED����                  
}



