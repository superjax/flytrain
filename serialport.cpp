#include "serialport.h"

SerialPort::SerialPort(){}

void SerialPort::init(USART_TypeDef *hardware, uint32_t baudrate, GPIO_TypeDef port,
                      uint16_t rx_pin, uint16_t tx_pin, uint8_t mode, IRQn_Type IRQn)
{
    // Configure UART GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin       = USART1_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USART1_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART1_RX_PIN;
    HAL_GPIO_Init(USART1_GPIO, &GPIO_InitStruct);
}
