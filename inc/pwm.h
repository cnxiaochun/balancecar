#pragma once

#include <stdint.h>

/**
 * @brief 最大PWM输出
 */
#define PWM_MAX_VALUE   4095

/**
 * @biref PWM初始化.
 */
void pwm_init(void);

/**
 * @brief 左轮PWM输出.
 *
 * @param value 输出值, <= PWM_MAX_VALUE
 */
void pwm_l(uint16_t value);

/**
 * @brief 右轮PWM输出.
 *
 * @param value 输出值, <= PWM_MAX_VALUE
 */
void pwm_r(uint16_t value);

