#include "CH455.h"

/* ================= 构造函数 ================= */

CH455::CH455(uint8_t scl, uint8_t sda, Polarity polarity)
    : _scl(scl), _sda(sda), _polarity(polarity)
{
}

/* ================= 公共接口 ================= */

void CH455::begin(uint8_t brightness)
{
    pinMode(_scl, OUTPUT);
    pinMode(_sda, OUTPUT);
    digitalWrite(_scl, HIGH);
    digitalWrite(_sda, HIGH);

    setBrightness(brightness);
    clear();
}

void CH455::setBrightness(uint8_t level)
{
    if (level < 1) level = 1;
    if (level > 8) level = 8;

    static const uint8_t intensTable[8] = {
        CH455_BIT_INTENS1,
        CH455_BIT_INTENS2,
        CH455_BIT_INTENS3,
        CH455_BIT_INTENS4,
        CH455_BIT_INTENS5,
        CH455_BIT_INTENS6,
        CH455_BIT_INTENS7,
        CH455_BIT_INTENS8
    };

    writeRaw(CH455_SYSON | intensTable[level - 1]);
}

void CH455::displayNumber(int value, bool leadingZero)
{
    static const uint8_t segTable[10] = {
        0x3F, 0x06, 0x5B, 0x4F, 0x66,
        0x6D, 0x7D, 0x07, 0x7F, 0x6F
    };

    if (value > 9999) value = 9999;
    if (value < -999) value = -999;

    int v = abs(value);

    for (int i = 0; i < 4; i++) {
        int d = v % 10;
        v /= 10;

        if (v == 0 && d == 0 && !leadingZero && i > 0)
            writeDigit(3 - i, 0x00);
        else
            writeDigit(3 - i, segTable[d]);
    }
}

void CH455::clear()
{
    for (int i = 0; i < 4; i++)
        writeDigit(i, 0x00);
}

uint8_t CH455::readKey()
{
    uint8_t key;

    i2cStart();
    i2cWriteByte(((CH455_GET_KEY >> 7) & CH455_I2C_MASK) | 0x01 | CH455_I2C_ADDR);
    key = i2cReadByte();
    i2cStop();

    return key;
}

/* ================= 私有函数 ================= */

void CH455::writeRaw(uint16_t data)
{
    i2cStart();
    i2cWriteByte(((data >> 7) & CH455_I2C_MASK) | CH455_I2C_ADDR);
    i2cWriteByte(data & 0xFF);
    i2cStop();
}

void CH455::writeDigit(uint8_t index, uint8_t seg)
{
    uint16_t base;

    switch (index) {
        case 0: base = CH455_DIG0; break;
        case 1: base = CH455_DIG1; break;
        case 2: base = CH455_DIG2; break;
        case 3: base = CH455_DIG3; break;
        default: return;
    }

    writeRaw(base | encodeSeg(seg));
}

uint8_t CH455::encodeSeg(uint8_t raw)
{
    return (_polarity == COMMON_ANODE) ? ~raw : raw;
}

/* ================= 软件 I2C ================= */

void CH455::i2cDelay()
{
    delayMicroseconds(5);
}

void CH455::i2cStart()
{
    pinMode(_sda, OUTPUT);
    digitalWrite(_sda, HIGH);
    digitalWrite(_scl, HIGH);
    i2cDelay();
    digitalWrite(_sda, LOW);
    i2cDelay();
    digitalWrite(_scl, LOW);
}

void CH455::i2cStop()
{
    pinMode(_sda, OUTPUT);
    digitalWrite(_sda, LOW);
    digitalWrite(_scl, LOW);
    i2cDelay();
    digitalWrite(_scl, HIGH);
    i2cDelay();
    digitalWrite(_sda, HIGH);
}

void CH455::i2cWriteByte(uint8_t data)
{
    pinMode(_sda, OUTPUT);

    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(_sda, (data & 0x80) ? HIGH : LOW);
        i2cDelay();
        digitalWrite(_scl, HIGH);
        i2cDelay();
        digitalWrite(_scl, LOW);
        data <<= 1;
    }

    // 忽略 ACK
    pinMode(_sda, INPUT);
    i2cDelay();
    digitalWrite(_scl, HIGH);
    i2cDelay();
    digitalWrite(_scl, LOW);
}

uint8_t CH455::i2cReadByte()
{
    uint8_t data = 0;
    pinMode(_sda, INPUT);

    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(_scl, HIGH);
        i2cDelay();
        data <<= 1;
        if (digitalRead(_sda)) data |= 1;
        digitalWrite(_scl, LOW);
        i2cDelay();
    }

    // NACK
    pinMode(_sda, OUTPUT);
    digitalWrite(_sda, HIGH);
    digitalWrite(_scl, HIGH);
    i2cDelay();
    digitalWrite(_scl, LOW);

    return data;
}
