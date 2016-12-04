#include "serialport.h"

SerialPort* UART1Ptr = NULL;
SerialPort* UART2Ptr = NULL;
SerialPort* UART3Ptr = NULL;

SerialPort::SerialPort(int index)
{
    txCompleteCB_ = NULL;
    rxCompleteCB_ = NULL;
    errorCB_ = NULL;

    rx_buffer_head_ = rx_buffer_tail_ = 0;
    tx_buffer_head_ = tx_buffer_tail_ = 0;

    // Initialze RCC stuff
    if(index == 1)
    {
        hardware_ = USART1;
        TURN_ON_USART1();
        TURN_ON_USART1_DMA();
        GPIO_ = USART1_GPIO;
        rx_pin_ = USART1_RX_PIN;
        tx_pin_ = USART1_TX_PIN;
        UART_IRQn_ = USART1_IRQn;
        Rx_DMA_IRQn_ = USART1_DMA_RX_IRQn;
        Tx_DMA_IRQn_ = USART1_DMA_TX_IRQn;
        Rx_DMA_Channel_ = USART1_DMA_RX;
        Tx_DMA_Channel_ = USART1_DMA_TX;
        UART1Ptr = this;
    }
    else if(index == 2)
    {
        hardware_ = USART2;
        TURN_ON_USART2();
        TURN_ON_USART2_DMA();
        GPIO_ = USART2_GPIO;
        rx_pin_ = USART2_RX_PIN;
        tx_pin_ = USART2_TX_PIN;
        UART_IRQn_ = USART2_IRQn;
        Rx_DMA_IRQn_ = USART2_DMA_RX_IRQn;
        Tx_DMA_IRQn_ = USART2_DMA_TX_IRQn;
        Rx_DMA_Channel_ = USART2_DMA_RX;
        Tx_DMA_Channel_ = USART2_DMA_TX;
        UART2Ptr = this;
    }
    else if(index == 3)
    {
        hardware_ = USART3;
        TURN_ON_USART3();
        TURN_ON_USART3_DMA();
        GPIO_ = USART3_GPIO;
        rx_pin_ = USART3_RX_PIN;
        tx_pin_ = USART3_TX_PIN;
        UART_IRQn_ = USART3_IRQn;
        Rx_DMA_IRQn_ = USART3_DMA_RX_IRQn;
        Tx_DMA_IRQn_ = USART3_DMA_TX_IRQn;
        Rx_DMA_Channel_ = USART3_DMA_RX;
        Tx_DMA_Channel_ = USART3_DMA_TX;
        UART3Ptr = this;
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

    // Configure DMA Channels
    static DMA_HandleTypeDef hdma_tx;
    static DMA_HandleTypeDef hdma_rx;
    if(mode_ == MODE_DMA)
    {
        // Tx DMA
        hdma_tx.Instance = Tx_DMA_Channel_;
        hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_tx.Init.Mode = DMA_NORMAL;
        hdma_tx.Init.Priority = DMA_PRIORITY_LOW;

        HAL_DMA_Init(&hdma_tx);
        __HAL_LINKDMA(&UartHandle_, hdmatx, hdma_tx);

        // RX DMA
        hdma_rx.Instance = Rx_DMA_Channel_;
        hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_rx.Init.Mode = DMA_NORMAL;
        hdma_rx.Init.Priority = DMA_PRIORITY_HIGH;

        HAL_DMA_Init(&hdma_rx);
        __HAL_LINKDMA(&UartHandle_, hdmarx, hdma_rx);

        // Set up the DMA Interrupts
        HAL_NVIC_SetPriority(Rx_DMA_IRQn_, 0, 1);
        HAL_NVIC_EnableIRQ(Rx_DMA_IRQn_);

        HAL_NVIC_SetPriority(Tx_DMA_IRQn_, 0, 0);
        HAL_NVIC_EnableIRQ(Tx_DMA_IRQn_);

        // Set up UART interrupts
        HAL_NVIC_SetPriority(UART_IRQn_, 0, 1);
        HAL_NVIC_EnableIRQ(UART_IRQn_);
    }

    // Configure the Interrupts
    if(mode_ == MODE_INTERRUPT)
    {
        // Set Up NVIC for USART interrupts
        HAL_NVIC_SetPriority(UART_IRQn_, 0, 1);
        HAL_NVIC_EnableIRQ(UART_IRQn_);
    }

    // Configure UART Hardware
    UartHandle_.Instance = hardware_;
    UartHandle_.Init.BaudRate = baudrate_;
    UartHandle_.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle_.Init.StopBits = UART_STOPBITS_1;
    UartHandle_.Init.Parity = UART_PARITY_NONE;
    UartHandle_.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle_.Init.Mode = UART_MODE_TX_RX;

    if(HAL_UART_Init(&UartHandle_) != HAL_OK)
    {
        error_CB();
    }
}

HAL_StatusTypeDef SerialPort::write(uint8_t *buffer, uint32_t len)
{

    if(mode_ == MODE_POLLING)
    {
        // There is no circular buffer for the polling mode, so just return
        while(HAL_UART_GetState(&UartHandle_) != HAL_UART_STATE_READY)
        {
            ; // wait for the UART to become ready
        }
        return HAL_UART_Transmit(&UartHandle_, buffer, len, 0xFFFF);
    }

    // Put data on the circular buffer (move the head as you do it)
    for(int i = 0; i < len; i++)
    {
        tx_buffer_[tx_buffer_head_] = buffer[i];
        tx_buffer_head_ = (tx_buffer_head_ + 1) % UART_BUFFER_SIZE;
    }

    // If we are ready to send more data (the UART TX line was idle)
    if(!(HAL_UART_GetState(&UartHandle_) & HAL_UART_STATE_BUSY_TX ))
    {
        // Figure out the data to send
        uint8_t* buffer_to_send = &tx_buffer_[tx_buffer_tail_];
        uint32_t number_of_bytes = 0;
        if(tx_buffer_tail_ < tx_buffer_head_)
        {
            // this is the easy case, the bytes are all contiguous, so we can just send them all at once
            number_of_bytes = tx_buffer_head_ - tx_buffer_tail_;
            tx_buffer_tail_ = tx_buffer_head_;
        }
        else
        {
            // we have to send in two parts.  First the part to the end of the buffer, then the rest
            // will get sent on the complete callback
            number_of_bytes = UART_BUFFER_SIZE - tx_buffer_tail_;
            tx_buffer_tail_ = 0;
        }

        if(mode_ == MODE_INTERRUPT)
        {
            return HAL_UART_Transmit_IT(&UartHandle_, buffer_to_send, number_of_bytes);
        }
        else if(mode_ == MODE_DMA)
        {
            return HAL_UART_Transmit_DMA(&UartHandle_, buffer_to_send, number_of_bytes);
        }
    }
    else
    {
        volatile int debug = 1;
    }
}

HAL_StatusTypeDef SerialPort::read(uint8_t *buffer, uint32_t len)
{
    return HAL_UART_Receive(&UartHandle_, buffer, len, 0x1FFFF);
}

bool SerialPort::busy()
{
    HAL_UART_StateTypeDef current_state =  HAL_UART_GetState(&UartHandle_);
    if(current_state & HAL_UART_STATE_BUSY_RX)
    {
        return true;
    }
    return false;
}

void SerialPort::rx_complete_CB()
{
    if(rxCompleteCB_ != NULL)
        rxCompleteCB_();
}

void SerialPort::tx_complete_CB()
{
    // Check to make sure there isn't more data to send from the buffer
    if(tx_buffer_head_ != tx_buffer_tail_)
    {
        // Figure out the data to send
        uint8_t* buffer_to_send = &tx_buffer_[tx_buffer_tail_];
        uint32_t number_of_bytes = 0;
        if(tx_buffer_tail_ < tx_buffer_head_)
        {
            // this is the easy case, the bytes are all contiguous, so we can just send them all at once
            number_of_bytes = tx_buffer_head_ - tx_buffer_tail_;
            tx_buffer_tail_ = tx_buffer_head_;
        }
        else
        {
            // we have to send in two parts.  First the part to the end of the buffer, then the rest
            // will get sent on the complete callback
            number_of_bytes = UART_BUFFER_SIZE - tx_buffer_tail_;
            tx_buffer_tail_ = 0;
        }

        if(mode_ == MODE_INTERRUPT)
        {
            HAL_UART_Transmit_IT(&UartHandle_, buffer_to_send, number_of_bytes);
        }
        else if(mode_ == MODE_DMA)
        {
            HAL_UART_Transmit_DMA(&UartHandle_, buffer_to_send, number_of_bytes);
        }
    }
    else if(txCompleteCB_ != NULL)
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
    if(huart == &UART1Ptr->UartHandle_)
    {
        UART1Ptr->tx_complete_CB();
    }
    else if(huart == &UART2Ptr->UartHandle_)
    {
        UART2Ptr->tx_complete_CB();
    }
    else if(huart == &UART3Ptr->UartHandle_)
    {
        UART3Ptr->tx_complete_CB();
    }
}
void rx_complete_IRQ(UART_HandleTypeDef* huart)
{
    if(huart == &UART1Ptr->UartHandle_)
    {
        UART1Ptr->rx_complete_CB();
    }
    else if(huart == &UART2Ptr->UartHandle_)
    {
        UART2Ptr->rx_complete_CB();
    }
    else if(huart == &UART3Ptr->UartHandle_)
    {
        UART3Ptr->rx_complete_CB();
    }
}
void error_IRQ(UART_HandleTypeDef* huart)
{
    if(huart == &UART1Ptr->UartHandle_)
    {
        UART1Ptr->error_CB();
    }
    else if(huart == &UART2Ptr->UartHandle_)
    {
        UART2Ptr->error_CB();
    }
    else if(huart == &UART3Ptr->UartHandle_)
    {
        UART3Ptr->error_CB();
    }
}


//===============================================================================
// C-based IRQ callbacks
extern "C"
{

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART1Ptr->UartHandle_);
}
void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART2Ptr->UartHandle_);
}
void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UART3Ptr->UartHandle_);
}
void DMA1_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(UART3Ptr->UartHandle_.hdmarx);
}
void DMA1_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(UART3Ptr->UartHandle_.hdmatx);
}
void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(UART1Ptr->UartHandle_.hdmatx);
}
void DMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(UART1Ptr->UartHandle_.hdmarx);
}
void DMA1_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(UART2Ptr->UartHandle_.hdmatx);
}
void DMA1_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(UART2Ptr->UartHandle_.hdmarx);
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
