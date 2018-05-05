#include "nvic.h"
#include "stm32f10x.h"

void nvic_init(void) {
	NVIC_InitTypeDef NVIC_InitStructure;


    /**
     *  ## 数字越小, 优先级越高
     *
     *  ## 优先级组别
     *
     *  | Group | bits   | Description                            |
     *  | ----- | ------ | -------------------------------------- |
     *  | 0     | ssss   | 所有4位响应优先级                      |
     *  | 1     | psss   | 最高1位抢占式优先级, 最低3位响应优先级 |
     *  | 2     | ppss   | 最高2位抢占式优先级, 最低2位响应优先级 |
     *  | 3     | ppps   | 最高3位抢占式优先级, 最低1位响应优先级 |
     *  | 4     | pppp   | 所有4位抢占式优先级                    |
     */

    /* 优先级组别 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* ENCODER_L */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* ENCODER_R */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Monitor */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure); 
}

