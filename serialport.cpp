#include "serialport.h"

SerialPort::SerialPort()
{
    txCompleteCB_ = NULL;
    rxCompleteCB_ = NULL;
    errorCB = NULL;
}

void SerialPort::init(USART_TypeDef *hardware, uint32_t baudrate, uint8_t mode)

{
    // Copy Structure into class
    hardware_ = hardware;
    mode_ = mode;
    baudrate_ = baudrate;

    // Initialze RCC stuff
    if(hardware == USART1)
    {
        TURN_ON_USART1();
        GPIO_ = USART1_GPIO;
        rx_pin_ = USART1_RX_PIN;
        tx_pin_ = USART1_TX_PIN;
        IRQn_ = USART1_IRQn;
    }
    else if(hardware == USART2)
    {
        TURN_ON_USART2();
        GPIO_ = USART2_GPIO;
        rx_pin_ = USART2_RX_PIN;
        tx_pin_ = USART2_TX_PIN;
        IRQn_ = USART2_IRQn;
    }
    else if(hardware == USART3)
    {
        TURN_ON_USART3();
        GPIO_ = USART3_GPIO;
        rx_pin_ = USART3_RX_PIN;
        tx_pin_ = USART3_TX_PIN;
        IRQn_ = USART3_IRQn;
    }

    // Configure UART GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin       = tx_pin_;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIO_, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = rx_pin_;
    HAL_GPIO_Init(GPIO_, &GPIO_InitStruct);

    // Configure Uart Hardware
    UartHandle_.Instance = hardware_;
    UartHandle_.Init.BaudRate = baudrate_;
    UartHandle_.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle_.Init.StopBits = UART_STOPBITS_1;
    UartHandle_.Init.Parity = UART_PARITY_NONE;
    UartHandle_.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle_.Init.Mode = UART_MODE_TX_RX;

    if(HAL_UART_DeInit(&UartHandle_) != HAL_OK)
    {
        if(errorCB != NULL)
        {
            errorCB();
        }
        else
        {
            while(1);
        }
    }
    if(HAL_UART_Init(&UartHandle_) != HAL_OK)
    {
        if(errorCB != NULL)
        {
            errorCB();
        }
        else
        {
            while(1);
        }
    }

    if(mode_ == MODE_INTERRUPT)
    {
        // Set Up NVIC for USART interrupts
        HAL_NVIC_SetPriority(IRQn_, 0, 1);
        HAL_NVIC_EnableIRQ(IRQn_);
    }
}

HAL_StatusTypeDef SerialPort::write(uint8_t *buffer, uint32_t len)
{
    if(mode_ == MODE_POLLING)
    {
        return HAL_UART_Transmit(&UartHandle_, buffer, len, 0xFFFF);
    }
    else if(mode_ == MODE_INTERRUPT)
    {
        return HAL_UART_Transmit_IT(&UartHandle_, buffer, len);
    }
}
