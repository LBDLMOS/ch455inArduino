# CH455 Arduino Library User Guide

## 1\. Library Overview

The CH455 is an LED driver IC that integrates **seven-segment display driving** and **matrix keypad scanning**, and is widely used in instruments, control panels, and similar applications.

This library follows **Arduino programming conventions** and fully encapsulates the low-level timing of the CH455. Users do not need to handle I²C details and can directly display numbers, adjust brightness, and read keys.

### Features

* Seven-segment display functionality has been fully tested and works reliably  
  (key-reading is not fully tested, but the protocol implementation should be correct)
* Supports **common-cathode / common-anode** seven-segment displays
* Supports **1–8 levels of brightness adjustment**

---

## 2\. Hardware Connection

Example using STM32F103 (Arduino Core):

| CH455 | MCU Pin   |
|------|-----------|
| SCL  | PB6       |
| SDA  | PB7       |
| VCC  | 5V / 3.3V |
| GND  | GND       |

---

## 3\. Classes and Enums

### Seven-segment Polarity Enum

```cpp
enum Polarity {
    COMMON\_CATHODE,  // Common-cathode display
    COMMON\_ANODE     // Common-anode display
};

---

## 4. Constructor

```cpp
CH455(uint8\_t sclPin, uint8\_t sdaPin, Polarity polarity);
```

### Parameters

| Parameter  | Description                    |
| ---------- | ------------------------------ |
| `sclPin`   | I²C clock pin                  |
| `sdaPin`   | I²C data pin                   |
| `polarity` | Display type (cathode / anode) |

### Example

```cpp
CH455 display(PB6, PB7, CH455::COMMON\_CATHODE);
```

---

## 5\. Initialization

```cpp
void begin(uint8\_t brightness = 8);
```

### Functionality

* Initializes software I²C pins
* Enables display and keypad functions
* Sets brightness level
* Clears the display

### Parameters

| Parameter    | Description            |
| ------------ | ---------------------- |
| `brightness` | Brightness level (1–8) |

---

## 6\. Brightness Control

```cpp
void setBrightness(uint8\_t level);
```

### Parameters

| Parameter | Description            |
| --------- | ---------------------- |
| `level`   | Brightness level (1–8) |

### Example

```cpp
display.setBrightness(4);
```

---

## 7\. Number Display

```cpp
void displayNumber(int value, bool leadingZero = false);
```

### Functionality

* Displays an integer directly
* Automatic digit mapping
* Automatic polarity handling
* Defaults to **4-digit display**
  (for 3-digit displays, multiply `value` by 10)

### Parameters

| Parameter     | Description               |
| ------------- | ------------------------- |
| `value`       | Integer (-999 ~ 9999)     |
| `leadingZero` | Show leading zeros or not |

### Example

```cpp
display.displayNumber(123);        // Display 123
display.displayNumber(7, true);    // Display 0007
```

---

## 8\. Clear Display

```cpp
void clear();
```

Turns off all segments.

---

## 9\. Key Reading

```cpp
uint8\_t readKey();
```

### Return Values

| Value  | Meaning        |
| ------ | -------------- |
| `0x00` | No key pressed |
| Other  | CH455 key code |

> Key codes depend on the keypad wiring. Refer to the CH455 datasheet.

---

## 10\. Examples

Included examples (in the `examples` folder):

* `BasicDisplay` – Basic number display
* `BrightnessTest` – Brightness adjustment
* `KeyScan` – Keypad scanning

---

## 11\. Notes

1. This library uses **software I²C** (does not use `Wire`)
2. **DO NOT use hardware I²C**
3. If PCB digit order changes, only internal mapping needs modification

---

# CH455 Arduino 库使用说明（中文）

## 一、库简介

CH455 是一款集 **数码管显示驱动** 与 **矩阵键盘扫描** 于一体的 LED 驱动芯片，广泛用于仪表、控制面板等场景。

本库基于 **Arduino 编程规范**，对 CH455 底层时序进行了完整封装，用户无需关心 I²C 细节，即可直接显示数字、调节亮度并读取按键。

### 特点

* 数码管显示功能已完整测试
  （按键读取尚未实测，但协议实现应无问题）
* 支持 **共阴 / 共阳数码管**
* 支持 **1~8 级亮度调节**

---

## 二、硬件连接

以 STM32F103（Arduino Core）为例：

| CH455 | MCU 引脚  |
| ----- | ------- |
| SCL   | PB6     |
| SDA   | PB7     |
| VCC   | 5V/3.3V |
| GND   | GND     |

---

## 三、枚举定义

```cpp
enum Polarity {
    COMMON\_CATHODE,  // 共阴数码管
    COMMON\_ANODE     // 共阳数码管
};
```

---

## 四、构造函数

```cpp
CH455(uint8\_t sclPin, uint8\_t sdaPin, Polarity polarity);
```

---

## 五、初始化

```cpp
void begin(uint8\_t brightness = 8);
```

---

## 六、亮度设置

```cpp
void setBrightness(uint8\_t level);
```

---

## 七、数字显示

```cpp
void displayNumber(int value, bool leadingZero = false);
```

---

## 八、清屏

```cpp
void clear();
```

---

## 九、按键读取

```cpp
uint8\_t readKey();
```

---

## 十、注意事项

1. 使用 **软件 I²C**
2. **禁止使用硬件 I²C**
3. PCB 位序变化仅需修改内部映射
