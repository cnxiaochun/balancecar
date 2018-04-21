/*
 * psx.c
 *
 *  Created on: 2018年2月5日
 *      Author: xiaochun
 */

#include "psx.h"
#include <stdint.h>
#include "stm32f10x.h"

#include "stmio.h"

#define PSX_PIN_CS              STMIO_PA08
#define PSX_PIN_MOSI            STMIO_PB14
#define PSX_PIN_CLK             STMIO_PB13
#define PSX_PIN_MISO            STMIO_PB15

/* PCB 32_CT */
// #define PSX_PIN_CS              STMIO_PA12
// #define PSX_PIN_MOSI            STMIO_PA13
// #define PSX_PIN_CLK             STMIO_PA11
// #define PSX_PIN_MISO            STMIO_PA14

/* CS */
#define SPI_CS_HI()             STMIO_BSRR(PSX_PIN_CS)
#define SPI_CS_LO()             STMIO_BRR(PSX_PIN_CS)

/* MOSI */
#define SPI_MOSI_HI()           STMIO_BSRR(PSX_PIN_MOSI)
#define SPI_MOSI_LO()           STMIO_BRR(PSX_PIN_MOSI)

/* CLK */
#define SPI_CLK_HI()            STMIO_BSRR(PSX_PIN_CLK)
#define SPI_CLK_LO()            STMIO_BRR(PSX_PIN_CLK)

/* MISO */
#define SPI_MISO_READ()         STMIO_IDR(PSX_PIN_MISO)

__attribute__ ((unused))
static const uint8_t ENTER_CONFIG[] = {
    0x01, 0x43, 0x00, 0x01, 0x00
};

__attribute__ ((unused))
static const uint8_t EXIT_CONFIG[] = {
    0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A
};

__attribute__ ((unused))
static const uint8_t SET_MODE[] = {
    0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00
};

__attribute__ ((unused))
static const uint8_t SET_BYTES_LARGE[] = {
    0x01, 0x4F, 0x00, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00
};

__attribute__ ((unused))
static const uint8_t ENABLE_RUMBLE[] = {
    0x01, 0x4D, 0x00, 0x00, 0x01
};
 
__attribute__ ((unused))
static const uint8_t TYPE_READ[] = {
    0x01, 0x45, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A
};

static const uint8_t READ_KEYBOARD[] = {
    0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/**
 * @brief psx手柄初始化.
*/
void psx_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_StructInit(&GPIO_InitStructure);

    /* MISO = IPD */ 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    STMIO_INIT(PSX_PIN_MISO, &GPIO_InitStructure);

    /* CS, CLK, MOSI = Out_PP, 50MHz */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // IO 翻转 50MHz
    STMIO_INIT(PSX_PIN_CS, &GPIO_InitStructure);
    STMIO_INIT(PSX_PIN_CLK, &GPIO_InitStructure);
    STMIO_INIT(PSX_PIN_MOSI, &GPIO_InitStructure);
}

/**
 * @brief delay 10us.
 */
static void spi_delay(void) {
    int t;
    for (t = 0; t < 200; t++) {
    }
}

/**
 * @brief spi发送和接收数据.
 *
 * 时钟下降沿时，完成数据的发送与接收.
 *
 * SPI_MOSI     主机 -> 外设
 * SPI_MISO     外设 -> 主机
 * SPI_CLK      时钟
 *
 */
static uint8_t spi_transfer(uint8_t dat) {
    uint8_t result, i, msk;

    result = 0;
    for (i = 0; i < 8; i++) {
        msk = (uint8_t) (0x01 << i);
        if (dat & msk) {
            SPI_MOSI_HI();
        }
        else {
            SPI_MOSI_LO();
        }
        SPI_CLK_HI();
        spi_delay();
        SPI_CLK_LO();
        spi_delay();
        if (SPI_MISO_READ()) {
            result |= msk;
        }
        SPI_CLK_HI();
    }
    return result;
}

/**
 * @brief PSX手柄读写数据.
 *
 * @param wr_buf 需要写入的数据.
 * @param rd_buf 读取的数据.
 * @param size 写入和读取的数据长度.
 *
 */
static void _psx_read_write(const uint8_t * wr_buf, uint8_t * rd_buf, uint8_t size) {
    uint8_t i;

    SPI_CS_LO();                                         /* 默认低电平选中 */
    for (i = 0; i < size; i++) {
        rd_buf[i] = spi_transfer(wr_buf[i]);
    }
    SPI_CS_HI();                                         /* 默认低电平非选中 */
}

/**
 * @brief PSX手柄读按键信息.
 *
 * @param size = 数据大小, 9
 */
static void _psx_read_key(uint8_t * rd_buf, uint8_t size) {
    _psx_read_write(READ_KEYBOARD, rd_buf, size > sizeof(READ_KEYBOARD) ? sizeof(READ_KEYBOARD) : size);
}

static int _psx_changed(uint16_t last_state, uint16_t now_state) {
   return (last_state ^ now_state) > 0 ? 1 : 0;
}

static int _psx_button_changed(uint16_t last_state, uint16_t now_state, uint16_t button) {
   return ((last_state ^ now_state) & button) > 0 ? 1 : 0;
}

static int _psx_button_state(uint16_t state, uint16_t button) {
    return (~state & button) > 0 ? 1 : 0;
}

static int _psx_button_pressed(uint16_t last_state, uint16_t now_state, uint16_t button) {
    return _psx_button_changed(last_state, now_state, button) && _psx_button_state(now_state, button) ? 1 : 0;
}

static int _psx_button_released(uint16_t last_state, uint16_t now_state, uint16_t button) {
    return _psx_button_changed(last_state, now_state, button) && _psx_button_state(last_state, button) ? 1 : 0;
}

static uint8_t psx_buf[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

static uint16_t last_state = 0xffff;        /* 初始化全部released状态 */

static uint16_t now_state = 0xffff;

/**
 * @brief 查询psx按键状态.
 *
 * @return  1 psx查询成功.
 *          0 psx查询失败.
 */
int psx_query(void) {
    last_state = now_state;

    _psx_read_key(psx_buf, sizeof(psx_buf));

    if (0x5A == psx_buf[2]) {
        if (0x41 == psx_buf[1] || 0x73 == psx_buf[2]) {
            now_state = (uint16_t) ((psx_buf[4] << 8) | psx_buf[3]);
            if (0x41 == psx_buf[1]) {
                /* L3, R3 无效 */
            }
            return 1;
        }
    }
    return 0;
}

/**
 * @brief 返回psx按键缓存区.
 *
 * @return  psx按键缓存区.
 */
const uint8_t * psx_buffer(void) {
    return (const uint8_t *) psx_buf;
}

/**
 * @brief 是否有按键变化.
 *
 * @return  1 有按键变化.
 *          0 无按键变化.
 */
int psx_changed(void) {
    return _psx_changed(last_state, now_state);
}

/**
 * @brief 指定按键是否有变化.
 *
 * @return  1 有按键变化.
 *          0 无按键变化.
 */
int psx_button_changed(uint16_t button) {
    return _psx_button_changed(last_state, now_state, button);
}

/**
 * @brierf 查询指定按键状态.
 *
 * @param button 查询哪个按键.
 *
 * @return  1 指定按键按下状态.
 *          0 指定按键释放状态.
 */
int psx_button_state(uint16_t button) {
    return _psx_button_state(now_state, button);
}

/**
 * @brief 指定按键是否刚被释放.
 *
 * @param button 查询哪个按键.
 *
 * @return  1 指定按键刚被释放.
 */
int psx_button_released(uint16_t button) {
    return _psx_button_released(last_state, now_state, button);
}

/**
 * @brief 指定按键是否刚被按下.
 *
 * @param button 查询哪个按键.
 *
 * @return  1 指定按键刚被按下.
 */
int psx_button_pressed(uint16_t button) {
    return _psx_button_pressed(last_state, now_state, button);
}

