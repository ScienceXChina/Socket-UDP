#ifndef __LED_H__
#define __LED_H__

#define rt_hw_led_on(a) GPIO_SetBits(GPIOD, 1 << (a));
#define rt_hw_led_off(a) GPIO_ResetBits(GPIOD, 1 << (a));

void rt_hw_led_init(void);

#endif

