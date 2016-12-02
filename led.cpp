#include "led.h"

#include "flip32plus.h"

LED LED1;
LED LED2;

LED::LED()
{
    blink_last_time_ = 0;
}

void LED::init(GPIO_TypeDef* port, uint16_t pin)
{
    port_ = port;
    pin_ = pin;
    // Assumes that the RCC Clock to the GPIO port has already
    // been enabled

    // Also assumes that AFIO configuration has already occurred
    // for pins that need to be remapped (such as the LEDs on the
    // naze32)

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = pin;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void LED::on()
{
    // LED is activated low
    HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_RESET);
}

void LED::off()
{
    // LED is active high
    HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_SET);
}

void LED::toggle()
{
    HAL_GPIO_TogglePin(port_, pin_);
}

void LED::blink(uint32_t ms)
{
    if(millis() - blink_last_time_ > ms)
    {
        blink_last_time_ = millis();
        toggle();
    }
}
