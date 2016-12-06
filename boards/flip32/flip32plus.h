#ifndef FLIP32PLUS_H
#define FLIP32PLUS_H

/**
  ******************************************************************************
  * @file    flip32plus.h
  * @author  James Jackson
  * @date    28 Nov 2016
  * @brief   This file contains definitions for flip32 LEDs
  *          peripherals and COM ports hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include <stdint.h>

// C includes
extern "C" {
#include "stm32f1xx_hal.h"
#include "printf.h"
}

// C++ Includes
#include "led.h"
#include "serialport.h"



//=================================================================================
// DMA Mapping for STM32F1

//                                                         DMA1
// Channel  |       1       |       2       |       3       |       4       |       5       |       6       |       7       |
// ADC      |      ADC1     |               |               |               |               |               |               |
// SPI/I2S  |               |   SPI1_RX     |   SPI1_TX     |   SPI2/I2S_RX |   SPI2/I2S_TX |               |               |
// USART    |               |   USART3_TX   |   USART3_RX   |   USART1_TX   |   USART1_RX   |   USART2_RX   |   USART2_TX   |
// I2C      |               |               |               |   I2C2_TX     |   I2C2_RX     |   I2C1_TX     |   I2C2_RX     |

//                                                         DMA2
// Channel  |       1        |       2       |       3      |       4       |       5       |
// ADC      |                |               |              |               |   ADC3        |
// SPI/I2S  |   SPI3/I2S_RX  |  SPI3/I2S_TX  |              |               |               |
// UART     |                |               |  UART4_RX    |               |   UART4_TX    |


//#ifdef __cplusplus
//extern "C" {
//#endif

//#ifdef __cplusplus
//}
//#endif

//typedef enum
//{
//  LED1 = 0,
//  LED2 = 1,

//  LED_GREEN  = LED1,
//  LED_RED = LED2,

//} Led_TypeDef;

//typedef enum
//{
//  COM1 = 0,
//  COM2 = 1
//} COM_TypeDef;


 //=====================================================================================================
 // LED CONFIGURATION

#define LEDn                             2

#define LED1_PIN                         GPIO_PIN_4             /* PB.04*/
#define LED1_GPIO_PORT                   GPIOB

#define LED2_PIN                         GPIO_PIN_3            /* PB.03*/
#define LED2_GPIO_PORT                   GPIOB

//=====================================================================================================
// USART CONFIGURATION
#define USART1_GPIO                      GPIOA
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_DMA_RX                    DMA1_Channel5;
#define USART1_DMA_TX                    DMA1_Channel4;
#define USART1_DMA_RX_IRQn               DMA1_Channel5_IRQn;
#define USART1_DMA_TX_IRQn               DMA1_Channel4_IRQn;
#define TURN_ON_USART1_DMA()            __HAL_RCC_DMA1_CLK_ENABLE();
#define TURN_ON_USART1()                 __HAL_RCC_GPIOA_CLK_ENABLE(); \
                                         __HAL_RCC_USART1_CLK_ENABLE()

#define USART2_GPIO                     GPIOA
#define USART2_TX_PIN                   GPIO_PIN_2
#define USART2_RX_PIN                   GPIO_PIN_3
#define USART2_DMA_RX                   DMA1_Channel6;
#define USART2_DMA_TX                   DMA1_Channel7;
#define USART2_DMA_RX_IRQn              DMA1_Channel6_IRQn;
#define USART2_DMA_TX_IRQn              DMA1_Channel7_IRQn;
#define TURN_ON_USART2_DMA()            __HAL_RCC_DMA1_CLK_ENABLE();
#define TURN_ON_USART2()                __HAL_RCC_GPIOA_CLK_ENABLE(); \
                                        __HAL_RCC_USART2_CLK_ENABLE()

#define USART3_GPIO                     GPIOB
#define USART3_TX_PIN                   GPIO_PIN_10
#define USART3_RX_PIN                   GPIO_PIN_11
#define USART3_DMA_RX                   DMA1_Channel3;
#define USART3_DMA_TX                   DMA1_Channel2;
#define USART3_DMA_RX_IRQn              DMA1_Channel3_IRQn;
#define USART3_DMA_TX_IRQn              DMA1_Channel2_IRQn;
#define TURN_ON_USART3_DMA()            __HAL_RCC_DMA1_CLK_ENABLE();
#define TURN_ON_USART3()                __HAL_RCC_GPIOB_CLK_ENABLE(); \
                                        __HAL_RCC_USART3_CLK_ENABLE()



 //======================================================================================================
 // OTHER
#define IOE_IT_PIN                       GPIO_PIN_14
#define IOE_IT_GPIO_PORT                 GPIOB
#define IOE_IT_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define IOE_IT_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()
#define IOE_IT_EXTI_IRQn                 EXTI15_10_IRQn
#define IOE_IT_EXTI_IRQHANDLER           EXTI15_10_IRQHandler

/* Exported constant IO ------------------------------------------------------*/
#define IO1_I2C_ADDRESS                       0x82
#define IO2_I2C_ADDRESS                       0x88
#define TS_I2C_ADDRESS                        0x82

/*The Slave ADdress (SAD) associated to the LIS302DL is 001110xb. SDO pad can be used
to modify less significant bit of the device address. If SDO pad is connected to voltage
supply LSb is �1� (address 0011101b) else if SDO pad is connected to ground LSb value is
�0� (address 0011100b).*/
#define L1S302DL_I2C_ADDRESS                  0x38


/*##################### ACCELEROMETER ##########################*/
/* Read/Write command */
#define READWRITE_CMD                     ((uint8_t)0x80)
/* Multiple byte read/write command */
#define MULTIPLEBYTE_CMD                  ((uint8_t)0x40)

/*##################### I2Cx ###################################*/
/* User can use this section to tailor I2Cx instance used and associated
   resources */
/* Definition for I2Cx Pins */
#define EVAL_I2Cx_SCL_PIN                       GPIO_PIN_6        /* PB.06*/
#define EVAL_I2Cx_SCL_GPIO_PORT                 GPIOB
#define EVAL_I2Cx_SDA_PIN                       GPIO_PIN_7        /* PB.07*/
#define EVAL_I2Cx_SDA_GPIO_PORT                 GPIOB

/* Definition for I2Cx clock resources */
#define EVAL_I2Cx                               I2C1
#define EVAL_I2Cx_CLK_ENABLE()                  __HAL_RCC_I2C1_CLK_ENABLE()
#define EVAL_I2Cx_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define EVAL_I2Cx_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()

#define EVAL_I2Cx_FORCE_RESET()                 __HAL_RCC_I2C1_FORCE_RESET()
#define EVAL_I2Cx_RELEASE_RESET()               __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx's NVIC */
#define EVAL_I2Cx_EV_IRQn                       I2C1_EV_IRQn
#define EVAL_I2Cx_EV_IRQHandler                 I2C1_EV_IRQHandler
#define EVAL_I2Cx_ER_IRQn                       I2C1_ER_IRQn
#define EVAL_I2Cx_ER_IRQHandler                 I2C1_ER_IRQHandler

/* I2C clock speed configuration (in Hz) */
#ifndef BSP_I2C_SPEED
 #define BSP_I2C_SPEED                            400000
#endif /* I2C_SPEED */


/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define EVAL_I2Cx_TIMEOUT_MAX                   3000

/*##################### SPI3 ###################################*/
#define EVAL_SPIx                               SPI3
#define EVAL_SPIx_CLK_ENABLE()                  __HAL_RCC_SPI3_CLK_ENABLE()

#define EVAL_SPIx_SCK_GPIO_PORT                 GPIOC             /* PC.10*/
#define EVAL_SPIx_SCK_PIN                       GPIO_PIN_10
#define EVAL_SPIx_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define EVAL_SPIx_SCK_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()

#define EVAL_SPIx_MISO_MOSI_GPIO_PORT           GPIOC
#define EVAL_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define EVAL_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOC_CLK_DISABLE()
#define EVAL_SPIx_MISO_PIN                      GPIO_PIN_11       /* PC.11*/
#define EVAL_SPIx_MOSI_PIN                      GPIO_PIN_12       /* PC.12*/
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define EVAL_SPIx_TIMEOUT_MAX                   1000

//========================================================================================
// Functions
void hardware_init(void);
void start_wall_clock(void);

uint32_t millis(void);
uint64_t micros(void);
void delay_ms(uint32_t ms);
void delay_us(uint64_t us);


/**
  * @}
  */

/**
  * @}_
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



#endif // FLIP32PLUS_H
