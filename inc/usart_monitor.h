#pragma once

#include <stdint.h>

/* 串口监控初始化 */
void usart_monitor_init(void);

/* 串口监控发送数据 */
void usart_monitor_send(const uint8_t * buf, uint8_t size);

/* 串口监控发送字符串 */
void usart_monitor_send_str(const char * buf);

/* 串口监控中断程序 */
void USART1_IRQHandler(void);

