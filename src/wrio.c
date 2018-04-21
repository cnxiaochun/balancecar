#include "wrio.h"
#include <stdint.h>
#include "stm32f10x.h"

#include "stmio.h"

#pragma pack(1)
struct PinTypedef {
    GPIO_TypeDef * gpio;
    uint32_t rcc;
    uint16_t pin;
};
#pragma pack()

static const struct PinTypedef wrio_list [WRIO_COUNT] = {
    {STMIO_GPIO_RCC_PIN(STMIO_PB12)},                   /* LED */
    {STMIO_GPIO_RCC_PIN(STMIO_PA00)},                   /* BEEP */
    {STMIO_GPIO_RCC_PIN(STMIO_PB08)},                   /* AN1 */
    {STMIO_GPIO_RCC_PIN(STMIO_PB09)},                   /* AN2 */
    {STMIO_GPIO_RCC_PIN(STMIO_PB05)},                   /* BN1 */
    {STMIO_GPIO_RCC_PIN(STMIO_PB04)}                    /* BN2 */
};

/**
 * @brief 输出端口初始化.
 */
void wrio_init(void) {
    int i;
    uint32_t RCC_APB2Periph;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2Periph = RCC_APB2Periph_AFIO;
    for (i = 0; i < (int) (sizeof(wrio_list) / sizeof(wrio_list[0])); i++) {
        RCC_APB2Periph |= wrio_list[i].rcc;
    }
    // Enable GPIO Peripheral clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);

    /**
     * GPIO_Remap_SWJ_NoJTRST      : Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST
     *      PA13, PA14, PA15, PB3不可用,  PB4可用
     *
     * GPIO_Remap_SWJ_JTAGDisable  : JTAG-DP Disabled and SW-DP Enabled
     *      PA13, PA14不可用        PA15, PB3, PB4可用
     *
     * GPIO_Remap_SWJ_Disable      : Full SWJ Disabled (JTAG-DP + SW-DP)
     *      PA13, PA14, PA15, PB3, PB4可用
     */
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // 使能禁止JTAG, 因为用到PB4

    // Configure pin in output push/pull mode, 2MHz
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    for (i = 0; i < (int) (sizeof(wrio_list) / sizeof(wrio_list[0])); i++) {
        GPIO_InitStructure.GPIO_Pin = wrio_list[i].pin;
        GPIO_Init(wrio_list[i].gpio, &GPIO_InitStructure);
    }

    // Start with led turned off
    for (i = 0; i < (int) (sizeof(wrio_list) / sizeof(wrio_list[0])); i++) {
        wrio_low(i);
    }
}

/**
 * @brief 指定端口高电平输出.
 *
 * @param index 端口号.
 */
void wrio_hig(int index) {
    if (index >= 0 && index < WRIO_COUNT) {
        wrio_list[index].gpio->BSRR = wrio_list[index].pin;
    }
}

/**
 * @brief 指定端口低电平输出.
 *
 * @param index 端口号.
 */
void wrio_low(int index) {
    if (index >= 0 && index < WRIO_COUNT) {
        wrio_list[index].gpio->BRR = wrio_list[index].pin;
    }
}
