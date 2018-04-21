#include "led.h"
#include "wrio.h"
#include "stimer.h"

/* 当前led灯状态 */
static int led_state;

/* 当前蜂鸣器状态 */
static int beep_state;

/* 蜂鸣器响几下 */
static int beep_count;

/**
 * @brief led事件.
 * 
 * 主循环中调用, 1秒中亮灭1次.
 */
void led_event(void) {
    if (StimerCheck(TIMER_LED)) {
        if (led_state) {
            led_state = 0;
            led_off(WRIO_LED);
        }
        else {
            led_state = 1;
            led_on(WRIO_LED);
        }
        StimerRun(TIMER_LED, ms2tick(500));
    }
}

/**
 * @brief 蜂鸣器事件.
 *
 * 主循环中调用.
 */
void beep_event(void) {
    if (StimerCheck(TIMER_BEEP)) {
        if (beep_state) {
            beep_state = 0;
            wrio_hig(WRIO_BEEP);                        /* stop beep */
            StimerRun(TIMER_BEEP, ms2tick(200));
        }
        else if (beep_count > 0) {
            beep_count--;
            beep_state = 1;
            wrio_low(WRIO_BEEP);                        /* start beep */
            StimerRun(TIMER_BEEP, ms2tick(100));
        }
    }
}

/**
 * @breif 点亮指定led灯.
 *
 * @param index led灯编号.
 */
void led_on(int index) {
    wrio_hig(index);
}

/**
 * @breif 熄灭指定led灯.
 *
 * @param index led灯编号.
 */
void led_off(int index) {
    wrio_low(index);
}

/**
 * @breif 蜂鸣器响count下
 *
 * @param count 响铃次数, 1 ~ 5
 */
void beep(int count) {
    if (count > 0 && count <= 5) {
        beep_count = count;
    }
}

