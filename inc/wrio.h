#pragma once

enum WrioEnum {
    WRIO_LED, 
    WRIO_BEEP,
    WRIO_AN1,
    WRIO_AN2,
    WRIO_BN1,
    WRIO_BN2,
    WRIO_COUNT
};

/* 输出端口初始化 */
void wrio_init(void);

/* 指定端口高电平输出 */
void wrio_hig(int index);

/* 指定端口低电平输出 */
void wrio_low(int index);

