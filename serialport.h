#ifndef SERIALPORT_H
#define SERIALPORT_H


class SerialPort
{

    enum
    {
        MODE_POLLING,
        MODE_INTERRUPT
    };
public:
    SerialPort();
    void init(USART_TypeDef* hardware, uint32_t baudrate, GPIO_TypeDef port, uint16_t rx_pin,
         uint16_t tx_pin, uint8_t mode, IRQn_Type IRQn);
    void write(uint8_t* buffer, uint32_t len);

    void register_tx_complete_CB(void);
    void register_rx_complete_CB(void);
    void register_error_CB(void);

private:
    UART_HandleTypeDef UartHandle_;
    GPIO_TypeDef GPIO_;
    uint16_t rx_pin_;
    uint16_t tx_pin_;
    uint8_t mode_;
    uint32_t baudrate_;

    void (*txCompleteCB_)(void);
    void (*rxCompleteCB_)(void);
    void (*errorCB)(void);
};

#endif // SERIALPORT_H
