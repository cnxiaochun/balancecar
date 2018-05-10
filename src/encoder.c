#include "encoder.h"
#include <stdint.h>
#include "stm32f10x.h"

#include "stmio.h"

/***************************************************
 * Encoder Left
 ***************************************************/
#define ENCODER_L_TIM           TIM4

#define ENCODER_L_TIM_RCC       RCC_APB1Periph_TIM4

#define ENCODER_L_PIN_A         STMIO_PB06

#define ENCODER_L_PIN_B         STMIO_PB07

/***************************************************
 * Encoder Right
 ***************************************************/
#define ENCODER_R_TIM           TIM3

#define ENCODER_R_TIM_RCC       RCC_APB1Periph_TIM3

#define ENCODER_R_PIN_A         STMIO_PA06

#define ENCODER_R_PIN_B         STMIO_PA07

#define ENCODER_MAX_VALUE       0x10000
/**
 * @breif AB编码器初始化.
 *
 */
void encoder_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    uint32_t RCC_APB2Periph;
  
    /* TIMx clock source enable */
    RCC_APB1PeriphClockCmd(ENCODER_L_TIM_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(ENCODER_R_TIM_RCC, ENABLE);

    /* GPIO clock enable */
    RCC_APB2Periph = RCC_APB2Periph_AFIO;
    RCC_APB2Periph |= STMIO_RCC_MASKx(ENCODER_L_PIN_A);
    RCC_APB2Periph |= STMIO_RCC_MASKx(ENCODER_L_PIN_B);
    RCC_APB2Periph |= STMIO_RCC_MASKx(ENCODER_R_PIN_A);
    RCC_APB2Periph |= STMIO_RCC_MASKx(ENCODER_R_PIN_B);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE); 
  
    GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    STMIO_INIT(ENCODER_L_PIN_A, &GPIO_InitStructure);
    STMIO_INIT(ENCODER_L_PIN_B, &GPIO_InitStructure);
    STMIO_INIT(ENCODER_R_PIN_A, &GPIO_InitStructure);
    STMIO_INIT(ENCODER_R_PIN_B, &GPIO_InitStructure);

    /* Timer configuration in Encoder mode */
    TIM_DeInit(ENCODER_L_TIM);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = ENCODER_MAX_VALUE - 1;  
    TIM_TimeBaseStructure.TIM_Prescaler = 0;    // No prescaling 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 设置时钟分频系数：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式 
    TIM_TimeBaseInit(ENCODER_L_TIM, &TIM_TimeBaseStructure);

    // TIM_ICPolarity_Rising上升沿捕获
    // 使用编码器模式3，上升下降都计数
    // TIM_EncoderInterfaceConfig(ENCODER_L_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    TIM_EncoderInterfaceConfig(ENCODER_L_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;              /* 滤波器 */
    TIM_ICInit(ENCODER_L_TIM, &TIM_ICInitStructure);
  
    // Clear all pending interrupts
    TIM_ClearFlag(ENCODER_L_TIM, TIM_FLAG_Update);
    TIM_ITConfig(ENCODER_L_TIM, TIM_IT_Update, ENABLE);	/* 使能中断 */

    // Reset counter
    ENCODER_L_TIM->CNT = 0;
  
    TIM_Cmd(ENCODER_L_TIM, ENABLE);  	                    /* 使能定时器 */

    /* Timer configuration in Encoder mode */
    TIM_DeInit(ENCODER_R_TIM);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 设置时钟分频系数：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式 
    TIM_TimeBaseInit(ENCODER_R_TIM, &TIM_TimeBaseStructure);

    // TIM_ICPolarity_Rising上升沿捕获
    // 使用编码器模式3，上升下降都计数
    // TIM_EncoderInterfaceConfig(ENCODER_R_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    TIM_EncoderInterfaceConfig(ENCODER_R_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;              /* 滤波器 */
    TIM_ICInit(ENCODER_R_TIM, &TIM_ICInitStructure);
  
    // Clear all pending interrupts
    TIM_ClearFlag(ENCODER_R_TIM, TIM_FLAG_Update);
    TIM_ITConfig(ENCODER_R_TIM, TIM_IT_Update, ENABLE);	/* 使能中断 */

    // Reset counter
    ENCODER_R_TIM->CNT = 0;
  
    TIM_Cmd(ENCODER_R_TIM, ENABLE);  	                /* 使能定时器 */
}

static void on_tim_irq(TIM_TypeDef* TIMx) {
    uint16_t tsr;
    tsr = TIMx->SR;	

    // 溢出中断
    if (tsr & 0x0001) {
    }				   
    TIMx->SR &= (uint16_t) ~(1 << 0);                   /* 清除中断标志位 */
}

static uint16_t get_encoder_count(TIM_TypeDef* TIMx) {
    uint16_t result;
    result = TIM_GetCounter(TIMx);                      /* TIMx定时器计算调用 */
    return result;
}

void TIM4_IRQHandler(void) { 		    		  			    
    on_tim_irq(ENCODER_L_TIM);
}

void TIM3_IRQHandler(void) { 		    		  			    
    on_tim_irq(ENCODER_R_TIM);
}

static uint16_t encoder_left;

static uint16_t encoder_right;


static int16_t encoder_speed(uint16_t current, uint16_t last) {
    int16_t result;
    if (current >= last) {
        result = current - last;
    }
    else {
        result = last - current;
        if (result > ENCODER_MAX_VALUE / 2) {
            /* 溢出了 */
            result = ENCODER_MAX_VALUE - value;
        }
        else {
            result = -result;
        }
    }
    return result;
}

/**
 * @brief 编码器读取任务.
 *
 * 周期: 5ms
 */
void on_encoder_task(int16_t *speed_l, int16_t *speed_r) {
    uint16_t value;
    int16_t speed;

    /* 左轮 */
    value = get_encoder_count(ENCODER_L_TIM);
    speed = encoder_speed(value, encoder_left);
    encoder_left = value;
    if (speed_l) *speed_l = speed;

    /* 右轮 */
    value = get_encoder_count(ENCODER_R_TIM);
    speed = encoder_speed(value, encoder_right);
    encoder_right = value;
    if (speed_r) *speed_r = speed;
}

uint16_t get_encoder_l_count(void) {
    return get_encoder_count(ENCODER_L_TIM);
}

uint16_t get_encoder_r_count(void) {
    return get_encoder_count(ENCODER_R_TIM);
}
