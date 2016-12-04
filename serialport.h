#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "flip32plus.h"

enum
{
    MODE_POLLING,
    MODE_INTERRUPT,
    MODE_DMA
};

class SerialPort
{
public:
    SerialPort(int index);
    void init(uint32_t baudrate, uint8_t mode);
    HAL_StatusTypeDef write(uint8_t* buffer, uint32_t len);

    void register_tx_complete_CB(void);
    void register_rx_complete_CB(void);
    void register_error_CB(void);

    UART_HandleTypeDef UartHandle_;

    void tx_complete_CB(void);
    void rx_complete_CB(void);
    void error_CB(void);

private:
    USART_TypeDef* hardware_;
    GPIO_TypeDef* GPIO_;
    uint16_t rx_pin_;
    uint16_t tx_pin_;
    uint8_t mode_;
    uint32_t baudrate_;
    IRQn_Type UART_IRQn_;
    IRQn_Type Tx_DMA_IRQn_;
    IRQn_Type Rx_DMA_IRQn_;
    DMA_Channel_TypeDef* Tx_DMA_Channel_;
    DMA_Channel_TypeDef* Rx_DMA_Channel_;

    void (*txCompleteCB_)(void);
    void (*rxCompleteCB_)(void);
    void (*errorCB_)(void);
};

extern SerialPort* UART1Ptr;
extern SerialPort* UART2Ptr;
extern SerialPort* UART2Ptr;

#endif // SERIALPORT_H
