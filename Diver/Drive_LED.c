#include "Drive_LED.h"

void LedGpioInit(void)
{
    // 定义GPIO的结构体变量
    GPIO_InitTypeDef GPIO_InitStructure;
    // 使能LED的GPIO对应的时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;         // 选择LED的引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 设置为推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 引脚反转速度设置为快

    // 初始化引脚配置
		GPIO_Init(GPIOB, &GPIO_InitStructure);
}