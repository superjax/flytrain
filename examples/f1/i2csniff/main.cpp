/*
 * main.cpp - I2C example for flip32
 * Copyright (c) 2016 James Jackson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "flip32plus.h"

SerialPort uart1(1);

static void _putc(void *p, char c)
{
    (void)p; // avoid compiler warning about unused variable
    uart1.write((uint8_t*)&c, 1);
}

int main(void)
{
    // Initializes LEDs and starts the wall clock
    hardware_init();

    // Initialize the UART for printing results of I2C sniffing
    uart1.init(9600, MODE_INTERRUPT);
    init_printf(NULL, _putc);

    // Start Hardware Clocks
    __HAL_RCC_I2C2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configure I2C pins
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin       = GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Configure I2C Peripheral
    I2C_HandleTypeDef I2cHandle;
    I2cHandle.Instance             = I2C2;
    I2cHandle.Init.OwnAddress1     = 0x00;
    I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    I2cHandle.Init.OwnAddress2     = 0x00;
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;

    if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
      {
        while(1);
      }

    // Toggle LED1 so they will alternate
    LED1.toggle();

    uint8_t buffer[1] = {0};
    for (uint8_t addr = 0; addr < 128; addr++)
    {
        if(HAL_I2C_Master_Transmit(&I2cHandle, addr, buffer, 1, 1) == HAL_OK)
        {
            printf("found address at %d", addr);
        }
        LED1.toggle();
        delay_ms(100);
        LED2.toggle();
        delay_us(100);
    }
}



