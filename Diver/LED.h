#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

/*
 *  函数名：void LedGpioInit(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：初始化LED的引脚，配置为上拉推挽输出
*/
void LedGpioInit(void);

#endif