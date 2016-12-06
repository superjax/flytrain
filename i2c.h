#ifndef I2C_H
#define I2C_H

#include "flip32plus.h"

class I2C
{
public:
    I2C(int index);

    bool write(uint8_t reg, uint8_t addr, uint8_t* buf, uint8_t len);
    bool writeDMA(uint8_t reg, uint8_t addr, uint8_t* buf, uint8_t len, void (*completeCB)(void));
    bool writeIT(uint8_t reg, uint8_t addr, uint8_t* buf, uint8_t len, void (*completeCB)(void));

    bool read(uint8_t reg, uint8_t addr, uint8_t* buf, uint8_t len);
    bool readDMA(uint8_t reg, uint8_t adr, uint8_t* buf, uint8_t len, void (*completeCB)(void));
    bool readIT(uint8_t reg, uint8_t adr, uint8_t* buf, uint8_t len, void (*completeCB)(void));

    enum
    {
        MODE_POLLING,
        MODE_INTERRUPT,
        MODE_DMA
    };

    I2C_TypeDef* hardware;

};

#endif // I2C_H
