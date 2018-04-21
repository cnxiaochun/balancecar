#define STMIO_PORT_A                    0
#define STMIO_PORT_B                    1
#define STMIO_PORT_C                    2
#define STMIO_PORT_D                    3
#define STMIO_PORT_E                    4
#define STMIO_PORT_F                    5
#define STMIO_PORT_G                    6

#define STMIO_PA00                      ((uint16_t) ((STMIO_PORT_A << 8) | 0))
#define STMIO_PA01                      ((uint16_t) ((STMIO_PORT_A << 8) | 1))
#define STMIO_PA02                      ((uint16_t) ((STMIO_PORT_A << 8) | 2))
#define STMIO_PA03                      ((uint16_t) ((STMIO_PORT_A << 8) | 3))
#define STMIO_PA04                      ((uint16_t) ((STMIO_PORT_A << 8) | 4))
#define STMIO_PA05                      ((uint16_t) ((STMIO_PORT_A << 8) | 5))
#define STMIO_PA06                      ((uint16_t) ((STMIO_PORT_A << 8) | 6))
#define STMIO_PA07                      ((uint16_t) ((STMIO_PORT_A << 8) | 7))
#define STMIO_PA08                      ((uint16_t) ((STMIO_PORT_A << 8) | 8))
#define STMIO_PA09                      ((uint16_t) ((STMIO_PORT_A << 8) | 9))
#define STMIO_PA10                      ((uint16_t) ((STMIO_PORT_A << 8) | 10))
#define STMIO_PA11                      ((uint16_t) ((STMIO_PORT_A << 8) | 11))
#define STMIO_PA12                      ((uint16_t) ((STMIO_PORT_A << 8) | 12))
#define STMIO_PA13                      ((uint16_t) ((STMIO_PORT_A << 8) | 13))
#define STMIO_PA14                      ((uint16_t) ((STMIO_PORT_A << 8) | 14))
#define STMIO_PA15                      ((uint16_t) ((STMIO_PORT_A << 8) | 15))

#define STMIO_PB00                      ((uint16_t) ((STMIO_PORT_B << 8) | 0))
#define STMIO_PB01                      ((uint16_t) ((STMIO_PORT_B << 8) | 1))
#define STMIO_PB02                      ((uint16_t) ((STMIO_PORT_B << 8) | 2))
#define STMIO_PB03                      ((uint16_t) ((STMIO_PORT_B << 8) | 3))
#define STMIO_PB04                      ((uint16_t) ((STMIO_PORT_B << 8) | 4))
#define STMIO_PB05                      ((uint16_t) ((STMIO_PORT_B << 8) | 5))
#define STMIO_PB06                      ((uint16_t) ((STMIO_PORT_B << 8) | 6))
#define STMIO_PB07                      ((uint16_t) ((STMIO_PORT_B << 8) | 7))
#define STMIO_PB08                      ((uint16_t) ((STMIO_PORT_B << 8) | 8))
#define STMIO_PB09                      ((uint16_t) ((STMIO_PORT_B << 8) | 9))
#define STMIO_PB10                      ((uint16_t) ((STMIO_PORT_B << 8) | 10))
#define STMIO_PB11                      ((uint16_t) ((STMIO_PORT_B << 8) | 11))
#define STMIO_PB12                      ((uint16_t) ((STMIO_PORT_B << 8) | 12))
#define STMIO_PB13                      ((uint16_t) ((STMIO_PORT_B << 8) | 13))
#define STMIO_PB14                      ((uint16_t) ((STMIO_PORT_B << 8) | 14))
#define STMIO_PB15                      ((uint16_t) ((STMIO_PORT_B << 8) | 15))

#define STMIO_PC00                      ((uint16_t) ((STMIO_PORT_C << 8) | 0))
#define STMIO_PC01                      ((uint16_t) ((STMIO_PORT_C << 8) | 1))
#define STMIO_PC02                      ((uint16_t) ((STMIO_PORT_C << 8) | 2))
#define STMIO_PC03                      ((uint16_t) ((STMIO_PORT_C << 8) | 3))
#define STMIO_PC04                      ((uint16_t) ((STMIO_PORT_C << 8) | 4))
#define STMIO_PC05                      ((uint16_t) ((STMIO_PORT_C << 8) | 5))
#define STMIO_PC06                      ((uint16_t) ((STMIO_PORT_C << 8) | 6))
#define STMIO_PC07                      ((uint16_t) ((STMIO_PORT_C << 8) | 7))
#define STMIO_PC08                      ((uint16_t) ((STMIO_PORT_C << 8) | 8))
#define STMIO_PC09                      ((uint16_t) ((STMIO_PORT_C << 8) | 9))
#define STMIO_PC10                      ((uint16_t) ((STMIO_PORT_C << 8) | 10))
#define STMIO_PC11                      ((uint16_t) ((STMIO_PORT_C << 8) | 11))
#define STMIO_PC12                      ((uint16_t) ((STMIO_PORT_C << 8) | 12))
#define STMIO_PC13                      ((uint16_t) ((STMIO_PORT_C << 8) | 13))
#define STMIO_PC14                      ((uint16_t) ((STMIO_PORT_C << 8) | 14))
#define STMIO_PC15                      ((uint16_t) ((STMIO_PORT_C << 8) | 15))

#define STMIO_GPIOx(_N)                 ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N >> 8)))
#define STMIO_RCC_MASKx(_N)             (RCC_APB2Periph_GPIOA << (_N >> 8))
#define STMIO_PIN_MASK(_N)              (1 << (_N & 0x00FF))
#define STMIO_GPIO_RCC_PIN(_N)          STMIO_GPIOx(_N), STMIO_RCC_MASKx(_N), STMIO_PIN_MASK(_N)

/* 指定端口低电平 */
#define STMIO_BRR(_N)                   do {STMIO_GPIOx(_N)->BRR = STMIO_PIN_MASK(_N);} while (0)

/* 指定端口高电平 */
#define STMIO_BSRR(_N)                  do {STMIO_GPIOx(_N)->BSRR = STMIO_PIN_MASK(_N);} while (0)

/* 如果低电平, 返回0, 高电平返回非0 */
#define STMIO_IDR(_N)                   (STMIO_GPIOx(_N)->IDR & STMIO_PIN_MASK(_N))

/* 指定端口初始化 */
#define STMIO_INIT(_N, InitType)                        \
    do {                                                \
        (InitType)->GPIO_Pin = STMIO_PIN_MASK(_N);      \
        GPIO_Init(STMIO_GPIOx(_N), (InitType));         \
    } while (0)

