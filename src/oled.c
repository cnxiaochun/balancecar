#include "oled.h"
#include "stm32f10x.h"

#include "stmio.h"

#define OLED_PIN_SCL            STMIO_PB06

#define OLED_PIN_SDA            STMIO_PB07

/**
 * @brief OLED器件地址.
 *
 * 通过调整0R电阻,屏可以0x78和0x7A两个地址
 * 默认0x78
 */
#define OLED_ADDRESS	        0x78

static void I2C_WriteByte(uint8_t addr, uint8_t data);

static void WriteCmd(uint8_t cmd);

static void WriteDat(uint8_t dat);

/**
 * @brief oled初始化.
 */
void oled_I2C_Configuration(void) {
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure; 

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;     /* I2C必须开漏输出 */
    STMIO_INIT(OLED_PIN_SCL, &GPIO_InitStructure);
    STMIO_INIT(OLED_PIN_SDA, &GPIO_InitStructure);

    I2C_DeInit(I2C1);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x30;           /* 主机的I2C地址,随便写的 */
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;          /* 400K */

    I2C_Cmd(I2C1, ENABLE);
    I2C_Init(I2C1, &I2C_InitStructure);
}

/**
 * @brief oled初始化.
 *
 * 上电后需延时100ms再初始化.
 */
void oled_init(void) {
    WriteCmd(0xAE); // display off
    WriteCmd(0x20); // Set Memory Addressing Mode    
    WriteCmd(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0); // Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8); // Set COM Output Scan Direction
    WriteCmd(0x00); // ---set low column address
    WriteCmd(0x10); // ---set high column address
    WriteCmd(0x40); // --set start line address
    WriteCmd(0x81); // --set contrast control register
    WriteCmd(0xff); // 亮度调节 0x00~0xff
    WriteCmd(0xa1); // --set segment re-map 0 to 127
    WriteCmd(0xa6); // --set normal display
    WriteCmd(0xa8); // --set multiplex ratio(1 to 64)
    WriteCmd(0x3F); // 
    WriteCmd(0xa4); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(0xd3); // -set display offset
    WriteCmd(0x00); // -not offset
    WriteCmd(0xd5); // --set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0); // --set divide ratio
    WriteCmd(0xd9); // --set pre-charge period
    WriteCmd(0x22); // 
    WriteCmd(0xda); // --set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb); // --set vcomh
    WriteCmd(0x20); // 0x20,0.77xVcc
    WriteCmd(0x8d); // --set DC-DC enable
    WriteCmd(0x14); // 
    WriteCmd(0xaf); // --turn on oled panel
}

/**
 * @brief 让OLED休眠.
 *
 * 休眠模式下, OLED功耗不到10uA
 */
void oled_sleep(void) {
    WriteCmd(0X8D);  // 设置电荷泵
    WriteCmd(0X10);  // 关闭电荷泵
    WriteCmd(0XAE);  // OLED休眠
}

/**
 * @brief 将OLED从休眠中唤醒.
 */
void oled_resume(void) {
    WriteCmd(0X8D);  // 设置电荷泵
    WriteCmd(0X14);  // 开启电荷泵
    WriteCmd(0XAF);  // OLED唤醒
}

/**
 * @brief 设置起始点坐标.
 */
void oled_pos(uint8_t x, uint8_t y) { 
    WriteCmd((uint8_t) (0xb0 + y));
    WriteCmd((uint8_t) (((x >> 4) & 0x0f) | 0x10));
    WriteCmd((uint8_t) ((x & 0x0f) | 0x01));
}

/**
 * @brief 全屏填充.
 */
void oled_fill(uint8_t fill_Data) {
    uint8_t m,n;
    for(m=0;m<8;m++) {
        WriteCmd((uint8_t) (0xb0 + m));      // page0-page1
        WriteCmd(0x00);        // low column start address
        WriteCmd(0x10);        // high column start address
        for(n=0;n<128;n++) {
            WriteDat(fill_Data);
        }
    }
}

/**
 * @brief 清屏.
 */
void oled_cls(void) {
    oled_fill(0x00);
}

static void I2C_WriteByte(uint8_t addr, uint8_t data) {
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    
    I2C_GenerateSTART(I2C1, ENABLE);                    /* 开启I2C1 */
    /* EV5,主模式 */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    /* 器件地址 -- 默认0x78 */
    I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, addr);                           /* 寄存器地址 */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C1, data);                           /* 发送数据 */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    I2C_GenerateSTOP(I2C1, ENABLE);                     /* 关闭I2C1总线 */
}

/**
 * @brief 向OLED写命令.
 */
static void WriteCmd(uint8_t cmd) {
    I2C_WriteByte(0x00, cmd);
}

/**
 * @brie 向OLED写数据.
 */
static void WriteDat(uint8_t dat) {
    I2C_WriteByte(0x40, dat);
}

