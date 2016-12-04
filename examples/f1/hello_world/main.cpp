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

UART_HandleTypeDef UartHandle;

int main(void)
{
    // Initializes LEDs and starts the wall clock
    hardware_init();

//    UART1.init(115200, MODE_INTERRUPT);

    // Configure UART GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin       = USART1_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USART1_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART1_RX_PIN;
    HAL_GPIO_Init(USART1_GPIO, &GPIO_InitStruct);

    // Configure the DMA'
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    DMA_HandleTypeDef DMA_Tx;
    DMA_HandleTypeDef DMA_Rx;

    DMA_Tx.Instance = DMA1_Channel4;
    DMA_Tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    DMA_Tx.Init.PeriphInc = DMA_PINC_DISABLE;
    DMA_Tx.Init.MemInc = DMA_MINC_ENABLE;
    DMA_Tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    DMA_Tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    DMA_Tx.Init.Mode = DMA_NORMAL;
    DMA_Tx.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&DMA_Tx);
    __HAL_LINKDMA(&UartHandle, hdmatx, DMA_Tx);

    DMA_Rx.Instance = DMA1_Channel5;
    DMA_Rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    DMA_Rx.Init.PeriphInc = DMA_PINC_DISABLE;
    DMA_Rx.Init.MemInc = DMA_MINC_ENABLE;
    DMA_Rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    DMA_Rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    DMA_Rx.Init.Mode = DMA_NORMAL;
    DMA_Rx.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&DMA_Rx);
    __HAL_LINKDMA(&UartHandle, hdmarx, DMA_Rx);

    // Configure NVIC
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);


    // Configure UART
    UartHandle.Instance        = USART1;;

    UartHandle.Init.BaudRate   = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;




    if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
    {
      while(1);
    }
    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
      while(1);
    }

    // Toggle LED1 so they will alternate
    LED1.toggle();


    while (1)
    {
        uint8_t test[12] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\n'};
//        if(UART1.write(test, 12) != HAL_OK)
//        {
//            while(1);
//        }
        if(HAL_UART_Transmit_DMA(&UartHandle, test, 12) != HAL_OK)
        {
            while(1);
        }
        LED1.toggle();
        delay_ms(100);
        LED2.toggle();
        delay_us(100);
    }
}


extern "C"
{

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UartHandle);
}

///**
//  * @brief  Tx Transfer completed callback
//  * @param  UartHandle: UART handle.
//  * @note   This example shows a simple way to report end of IT Tx transfer, and
//  *         you can add your own implementation.
//  * @retval None
//  */
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{

//}

///**
//  * @brief  Rx Transfer completed callback
//  * @param  UartHandle: UART handle
//  * @note   This example shows a simple way to report end of IT Rx transfer, and
//  *         you can add your own implementation.
//  * @retval None
//  */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{

//}

///**
//  * @brief  UART error callbacks
//  * @param  UartHandle: UART handle
//  * @note   This example shows a simple way to report transfer error, and you can
//  *         add your own implementation.
//  * @retval None
//  */
//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
//{
//    while(1);
//}

}



