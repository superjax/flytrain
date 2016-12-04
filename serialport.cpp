#include "serialport.h"

SerialPort UART1(1), UART2(2), UART3(3);

SerialPort::SerialPort(int index)
{
    txCompleteCB_ = NULL;
    rxCompleteCB_ = NULL;
    errorCB_ = NULL;

    // Initialze RCC stuff
    if(index == 1)
    {
        hardware_ = USART1;
        TURN_ON_USART1();
        GPIO_ = USART1_GPIO;
        rx_pin_ = USART1_RX_PIN;
        tx_pin_ = USART1_TX_PIN;
        IRQn_ = USART1_IRQn;
    }
    else if(index == 2)
    {
        hardware_ = USART2;
        TURN_ON_USART2();
        GPIO_ = USART2_GPIO;
        rx_pin_ = USART2_RX_PIN;
        tx_pin_ = USART2_TX_PIN;
        IRQn_ = USART2_IRQn;
    }
    else if(index == 3)
    {
        hardware_ = USART3;
        TURN_ON_USART3();
        GPIO_ = USART3_GPIO;
        rx_pin_ = USART3_RX_PIN;
        tx_pin_ = USART3_TX_PIN;
        IRQn_ = USART3_IRQn;
    }
}

void SerialPort::init(uint32_t baudrate, uint8_t mode)
{
    // Copy Structure into class
    mode_ = mode;
    baudrate_ = baudrate;

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
        error_CB();
    }
    if(HAL_UART_Init(&UartHandle_) != HAL_OK)
    {
        error_CB();
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

void SerialPort::rx_complete_CB()
{
    if(rxCompleteCB_ != NULL)
        rxCompleteCB_();
}

void SerialPort::tx_complete_CB()
{
    if(txCompleteCB_ != NULL)
        txCompleteCB_();
}

void SerialPort::error_CB()
{
    if(errorCB_ != NULL)
        errorCB_();
    else
        while(1);
}


//===============================================================================
// C to C++ Wrappers
// You can't call a class member function from within C, so I have to make
// These simple wrappers to transfer the Interrupt from the C IRQ to the
// class callback member
void tx_complete_IRQ(UART_HandleTypeDef* huart)
{
    if(huart == &UART1.UartHandle_)
    {
        UART1.tx_complete_CB();
    }
    else if(huart == &UART2.UartHandle_)
    {
        UART2.tx_complete_CB();
    }
    else if(huart == &UART3.UartHandle_)
    {
        UART3.tx_complete_CB();
    }
}
void rx_complete_IRQ(UART_HandleTypeDef* huart)
{
    if(huart == &UART1.UartHandle_)
    {
        UART1.rx_complete_CB();
    }
    else if(huart == &UART2.UartHandle_)
    {
        UART2.rx_complete_CB();
    }
    else if(huart == &UART3.UartHandle_)
    {
        UART3.rx_complete_CB();
    }
}
void error_IRQ(UART_HandleTypeDef* huart)
{
    if(huart == &UART1.UartHandle_)
    {
        UART1.error_CB();
    }
    else if(huart == &UART2.UartHandle_)
    {
        UART2.error_CB();
    }
    else if(huart == &UART3.UartHandle_)
    {
        UART3.error_CB();
    }
}


//===============================================================================
// C-based IRQ callbacks
extern "C"
{

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART1.UartHandle_);
}
void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART2.UartHandle_);
}
void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART3.UartHandle_);
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle.
  * @note   This example shows a simple way to report end of IT Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    tx_complete_IRQ(huart);
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    rx_complete_IRQ(huart);
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    error_IRQ(huart);
}

}
