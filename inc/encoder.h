#pragma once

#include <stdint.h>

/* AB编码器初始化 */
void encoder_init(void);

void TIM3_IRQHandler(void);

void TIM4_IRQHandler(void);

uint16_t get_encoder_l_count(void);

uint16_t get_encoder_r_count(void);

