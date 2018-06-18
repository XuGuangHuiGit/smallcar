#include "stm32f10x.h"
void brush_init(void);
void brush_pwm_init(void);
void brush_low(uint16_t brush);
void brush_high(uint16_t brush);
void set_speed(int speed_pwm);
