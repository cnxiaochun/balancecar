/*
 * usart_monitor.c
 *
 *  Created on: 2018年2月5日
 *      Author: xiaochun
 */

#include "usart_monitor.h"
#include <string.h>
#include "stm32f10x.h"

#include "stmio.h"

#define USART1_PIN_TX           STMIO_PA09
#define USART1_PIN_RX           STMIO_PA10

static uint8_t tx_pos;

static uint8_t tx_len;

static uint8_t tr_buf[128];

/**
 * @brief 串口监控初始化.
 */
void usart_monitor_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure; 	

	/* config USART clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* USART GPIO config */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    STMIO_INIT(USART1_PIN_TX, &GPIO_InitStructure);
	/* Configure USART Rx as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    STMIO_INIT(USART1_PIN_RX, &GPIO_InitStructure);

	/* USART mode config */
    USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;  
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;  
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;  
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  
    USART_ClockInit(USART1, &USART_ClockInitStructure);  
	USART_Init(USART1, &USART_InitStructure );   

    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief 串口监控发送数据.
 */
void usart_monitor_send(const void * buf, size_t size) {
    USART1->CR1 &= (uint16_t) ~(USART_FLAG_TXE);                   /* Disable the UART Transmit interrupt */

    tx_len = (uint8_t) (size > sizeof(tr_buf) ? sizeof(tr_buf) : size);
    memcpy(tr_buf, buf, tx_len);
    tx_pos = 0;

    /* Clear the Idle Line Detected bit in the status rerister by a read
       to the UART1_SR register followed by a Read to the UART1_DR register */
    (void) USART1->SR;
    (void) USART1->DR;

     USART1->CR1 |= USART_FLAG_TXE;                      /* Enable the UART Transmit interrupt */
}

/**
 * @brief 串口监控发送字符串.
 */
void usart_monitor_send_str(const char * buf) {
    usart_monitor_send((const uint8_t *) buf, strlen(buf));
}

/**
 * @brief 串口监控发送图表数据.
 */
void usart_monitor_send_chart(const void * buf, size_t size) {
    USART1->CR1 &= (uint16_t) ~(USART_FLAG_TXE);                   /* Disable the UART Transmit interrupt */

    tx_len = (uint8_t) (size > (sizeof(tr_buf) - 2) ? (sizeof(tr_buf) - 2) : size + 2);
    tr_buf[0] = 0xAA;
    tr_buf[1] = 0x55;
    memcpy(&tr_buf[2], (const void *) buf, size);
    tx_pos = 0;

    /* Clear the Idle Line Detected bit in the status rerister by a read
       to the UART1_SR register followed by a Read to the UART1_DR register */
    (void) USART1->SR;
    (void) USART1->DR;

     USART1->CR1 |= USART_FLAG_TXE;                      /* Enable the UART Transmit interrupt */
}

/**
 * @brief 串口监控中断程序.
 */
void USART1_IRQHandler(void) {
    if ((USART1->CR1 & USART_FLAG_TXE) && (USART_FLAG_TXE & USART1->SR)) {
        if(tx_pos < tx_len) {
            USART1->DR = tr_buf[tx_pos++];              /* Write one byte to the transmit data register */
        }
        if(tx_pos >= tx_len) {                          /* 数据全部发送完成 */
            USART1->CR1 &= (uint16_t) ~(USART_FLAG_TXE);/* Disable the UART Transmit interrupt */
        }
    }
}


