#include "Drive_LED.h"
#include "stm32F10x.h"

void delay(volatile long i) {
    while (i--);
}

int main(void) {
    LedGpioInit();
    while (1) {
        GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
        delay(1000000);
        GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
        delay(1000000);
    };
}
