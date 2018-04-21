#include <stdint.h>

void oled_I2C_Configuration(void);

void oled_init(void);

void oled_sleep(void);

void oled_resume(void);

void oled_pos(uint8_t x, uint8_t y);

void oled_fill(uint8_t fill_Data);

void oled_cls(void);

