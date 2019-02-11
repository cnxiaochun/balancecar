#pragma once

#include <stdint.h>

#define TIMER_FREQUENCY_HZ (1000u)

void systick_init(void);

void SysTick_Handler(void);

