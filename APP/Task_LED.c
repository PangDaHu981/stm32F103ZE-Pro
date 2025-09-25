#include "Task_LED.h"
#include "delay.h"
void LED_Task(void) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
    delay_ms(1000);
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
    delay_ms(1000);
}