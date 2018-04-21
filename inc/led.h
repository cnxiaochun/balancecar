#pragma once

/* led事件 */
void led_event(void);

/* 蜂鸣器事件 */
void beep_event(void);

/* 点亮指定led灯 */
void led_on(int index);

/* 熄灭指定led灯 */
void led_off(int index);

/* 蜂鸣器响count下 */
void beep(int count);

