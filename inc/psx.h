#pragma once

#include <stdint.h>

//These are our button constants
#define PSXB_SELECT      0x0001
#define PSXB_L3          0x0002
#define PSXB_R3          0x0004
#define PSXB_START       0x0008
#define PSXB_PAD_UP      0x0010
#define PSXB_PAD_RIGHT   0x0020
#define PSXB_PAD_DOWN    0x0040
#define PSXB_PAD_LEFT    0x0080
#define PSXB_L2          0x0100
#define PSXB_R2          0x0200
#define PSXB_L1          0x0400
#define PSXB_R1          0x0800
#define PSXB_GREEN       0x1000
#define PSXB_RED         0x2000
#define PSXB_BLUE        0x4000
#define PSXB_PINK        0x8000
#define PSXB_TRIANGLE    0x1000
#define PSXB_CIRCLE      0x2000
#define PSXB_CROSS       0x4000
#define PSXB_SQUARE      0x8000

/* PSX手柄初始化 */
void psx_init(void);

/* 查询psx按键状态 */
int psx_query(void);

/* 返回psx按键缓存区 */
const uint8_t * psx_buffer(void);

/* 是否有按键变化 */
int psx_changed(void);

/* 指定按键是否有变化 */
int psx_button_changed(uint16_t button);

/* 查询指定按键状态 */
int psx_button_state(uint16_t button);

/* 指定按键是否刚被释放 */
int psx_button_released(uint16_t button);

/* 指定按键是否刚被按下 */
int psx_button_pressed(uint16_t button);



