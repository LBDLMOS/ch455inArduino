#ifndef _CH455_H_
#define _CH455_H_

#include <Arduino.h>

/* ================= CH455 常量 ================= */

#define CH455_I2C_ADDR 0x40
#define CH455_I2C_MASK 0x3E

// 系统控制位
#define CH455_BIT_ENABLE   0x01
#define CH455_BIT_SLEEP    0x04
#define CH455_BIT_7SEG     0x08

#define CH455_BIT_INTENS1  0x10
#define CH455_BIT_INTENS2  0x20
#define CH455_BIT_INTENS3  0x30
#define CH455_BIT_INTENS4  0x40
#define CH455_BIT_INTENS5  0x50
#define CH455_BIT_INTENS6  0x60
#define CH455_BIT_INTENS7  0x70
#define CH455_BIT_INTENS8  0x00

#define CH455_SYSOFF   0x0400
#define CH455_SYSON    (CH455_SYSOFF | CH455_BIT_ENABLE)

// 数码管位
#define CH455_DIG0 0x1400
#define CH455_DIG1 0x1500
#define CH455_DIG2 0x1600
#define CH455_DIG3 0x1700

#define CH455_GET_KEY 0x0700

/* ================= 类定义 ================= */

class CH455 {
public:
    enum Polarity {
        COMMON_CATHODE,
        COMMON_ANODE
    };

    CH455(uint8_t scl, uint8_t sda, Polarity polarity = COMMON_CATHODE);

    void begin(uint8_t brightness = 8);
    void setBrightness(uint8_t level);

    void displayNumber(int value, bool leadingZero = false);
    void clear();

    uint8_t readKey();

private:
    uint8_t _scl;
    uint8_t _sda;
    Polarity _polarity;

    void i2cDelay();
    void i2cStart();
    void i2cStop();
    void i2cWriteByte(uint8_t data);
    uint8_t i2cReadByte();

    void writeRaw(uint16_t data);
    void writeDigit(uint8_t index, uint8_t seg);
    uint8_t encodeSeg(uint8_t raw);
};

#endif
