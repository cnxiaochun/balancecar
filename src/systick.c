#include "systick.h"
#include "cmsis_device.h"
#include "stimer.h"
#include "encoder.h"

void systick_init(void) {
    SysTick_Config(SystemCoreClock / TIMER_FREQUENCY_HZ);
}

void SysTick_Handler(void) {
    static uint8_t encoder_timer;

    stimer_task();

    if (encoder_timer >= (5 - 1)) {
        encoder_timer = 0;
        encoder_task();
    }
    else {
        encoder_timer++;
    }
}
