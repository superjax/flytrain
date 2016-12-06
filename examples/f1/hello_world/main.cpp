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

    uart1.init(9600, MODE_INTERRUPT);
    init_printf(NULL, _putc);

    // Toggle LED1 so they will alternate
    LED1.toggle();

    while (1)
    {
        printf("hello_world\n");
        LED1.toggle();
        delay_ms(100);
        LED2.toggle();
        delay_us(100);
    }
}



