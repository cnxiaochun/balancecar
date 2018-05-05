#include "pwm.h"
#include "stm32f10x.h"

#include "stmio.h"

/**
 * @brief PWM通道定义.
 *
 * TIM  | Channel | Pin  | Description
 * ---- | ------- | ---- | ------------
 * TIM2 | CH3     | PA02 | PWM L
 * TIM2 | CH4     | PA03 | PWM r
 *
 */

#define PWM_TIM             TIM2

#define PWM_TIM_RCC         RCC_APB1Periph_TIM2

/* Left motor PWM信号 */
#define PWM_L_PIN           STMIO_PA02

/* Right motor PWM信号 */
#define PWM_R_PIN           STMIO_PA03

#define TIM_PRESCALER       ((uint16_t) ((72000000 / 7200000) -1))

void pwm_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    uint32_t RCC_APB2Periph;
	
    /* TIM clock enable */
	RCC_APB1PeriphClockCmd(PWM_TIM_RCC, ENABLE);

    /* GPIO clock enable */
    RCC_APB2Periph = RCC_APB2Periph_AFIO;
    RCC_APB2Periph |= STMIO_RCC_MASKx(PWM_L_PIN);
    RCC_APB2Periph |= STMIO_RCC_MASKx(PWM_R_PIN);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE); 

    /* 引脚为复用推挽输出 */
    GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    STMIO_INIT(PWM_L_PIN, &GPIO_InitStructure);
    STMIO_INIT(PWM_R_PIN, &GPIO_InitStructure);

    /* 初始化TIM */
    TIM_DeInit(PWM_TIM);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = PWM_MAX_VALUE -1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_PRESCALER; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(PWM_TIM, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	// 初始化TIMx Channel3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(PWM_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);
	
	// 初始化TIMx Channel4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(PWM_TIM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(PWM_TIM, TIM_OCPreload_Enable); 

	TIM_Cmd(PWM_TIM, ENABLE);  //使能TIMx
}

void pwm_l(uint16_t value) {
    TIM_SetCompare3(PWM_TIM, value > PWM_MAX_VALUE ? PWM_MAX_VALUE : value);
}

void pwm_r(uint16_t value) {
    TIM_SetCompare4(PWM_TIM, value > PWM_MAX_VALUE ? PWM_MAX_VALUE : value);
}

