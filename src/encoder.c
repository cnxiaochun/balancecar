#include "encoder.h"
#include <stdint.h>
#include "stm32f10x.h"

#include "stmio.h"

#define ENCODER_PIN_A       STMIO_PB06

#define ENCODER_PIN_B       STMIO_PB07

/**
 * @breif AB编码器初始化.
 *
 */
void encoder_init(void) {
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  
    GPIO_InitTypeDef GPIO_InitStructure;
  
    /* TIM4 clock source enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* Enable GPIOA, clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
    GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    STMIO_INIT(ENCODER_PIN_A, &GPIO_InitStructure);
    STMIO_INIT(ENCODER_PIN_B, &GPIO_InitStructure);

    /* Enable the TIM4 Update Interrupt */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //优先级组别
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  
    /* Timer configuration in Encoder mode */
    TIM_DeInit(TIM4);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置时钟分频系数：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    //TIM_ICPolarity_Rising上升沿捕获
    //使用编码器模式3，上升下降都计数
    // TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10; //无滤波器
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
    // Clear all pending interrupts

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	  //使能中断

    // Reset counter
    TIM4->CNT = 0;
    // ENC_Clear_Speed_Buffer();
  
    TIM_Cmd(TIM4, ENABLE);  	   //使能定时器3
}

void TIM4_IRQHandler(void) { 		    		  			    
    uint16_t tsr;
    tsr = TIM4->SR;	

    // 溢出中断
    if (tsr & 0x0001) {
    }				   
    TIM4->SR &= (uint16_t) ~(1<<0);//清除中断标志位 	 
}

uint16_t get_encoder_count(void) {
    uint16_t result;
    result = TIM_GetCounter(TIM4);   //  TIM4定时器计算调用
    return result;
}
