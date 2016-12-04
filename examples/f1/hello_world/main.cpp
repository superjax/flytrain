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

#define USE_FULL_ASSERT

//#include "stm32f1xx_hal.h"
#include "flip32plus.h"

int main(void)
{
    // Initializes LEDs and starts the wall clock
    hardware_init();

    SerialPort uart1(1);
    uart1.init(921600, MODE_DMA);

    // Toggle LED1 so they will alternate
    LED1.toggle();


    while (1)
    {
        char *test = "hello world\n";
        uart1.write((uint8_t*)test, 12);
        LED1.toggle();
        delay_ms(100);
        LED2.toggle();
        delay_us(100);
    }
}



