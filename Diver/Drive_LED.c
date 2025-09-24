#include "Drive_LED.h"

void LedGpioInit(void)
{
    // ����GPIO�Ľṹ�����
    GPIO_InitTypeDef GPIO_InitStructure;
    // ʹ��LED��GPIO��Ӧ��ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;         // ѡ��LED������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // ����Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // ���ŷ�ת�ٶ�����Ϊ��

    // ��ʼ����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);
}