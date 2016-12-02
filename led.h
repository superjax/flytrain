#ifndef LED_H
#define LED_H

#include "flip32plus.h"
#include <stdint.h>
#include "stm32f1xx_hal.h"

class LED
{
public:
    LED();
    void init(GPIO_TypeDef* port, uint16_t pin);
    void on();
    void off();
    void toggle();
    void blink(uint32_t ms);

private:
    GPIO_TypeDef* port_;
    uint32_t pin_;
    uint32_t blink_last_time_;

};

extern LED LED1;
extern LED LED2;

#endif // LED_H
