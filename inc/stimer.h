#ifndef STIMER_H
#define STIMER_H

#include <stdint.h>

#define TIMER_FREQUENCY_HZ (1000u)

#define OS_STIMERS 20                       /* 静态定时器数量 */

enum OS_STIMERS_ENUM {
      TIMER_BEEP
    , TIMER_LED
    , TIMER_PSX
    , TIMER_ENCODER
};

void StimerRun(uint8_t stimer_id, uint32_t time);

void StimerBreak(uint8_t stimer_id);

void StimerPause(uint8_t stimer_id);

void StimerContinue(uint8_t stimer_id);

int StimerCheck(uint8_t stimer_id);

int StimerIsRun(uint8_t stimer_id);

void stimer_init(void);

void SysTick_Handler(void);

static inline uint32_t ms2tick(uint32_t time) {
    return time;
}

#endif
