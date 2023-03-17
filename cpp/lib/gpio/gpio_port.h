/**
 * @file gpio_port.h
 * @author Rohit Nimkar (nehalnimkar@gmail.com) <https://csrohit.github.io>
 * @brief Declaration of functionality related to GPIO Port
 * @version 1.2
 *
 * @copyright Copyright (c) 2023
 * @attention
 *
 * This software component is licensed by Rohit Nimkar under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at: opensource.org/licenses/BSD-3-Clause
 */

#include <stddef.h>
#include <stdint.h>

#include "stm32f1xx.h"

#ifndef __GPIO_H__
#define __GPIO_H__

class GPIO {
public:
enum Port { PortA = GPIOA_BASE, PortB = GPIOB_BASE, PortC = GPIOC_BASE };

/**
   * @brief GPIO port pins
   */
enum PIN : uint16_t {
    PIN_0 = (uint16_t)0x0000,  /* GPIO pin 0 */
    PIN_1 = (uint16_t)0x0001,  /* GPIO pin 1 */
    PIN_2 = (uint16_t)0x0002,  /* GPIO pin 2 */
    PIN_3 = (uint16_t)0x0003,  /* GPIO pin 3 */
    PIN_4 = (uint16_t)0x0004,  /* GPIO pin 4 */
    PIN_5 = (uint16_t)0x0005,  /* GPIO pin 5 */
    PIN_6 = (uint16_t)0x0006,  /* GPIO pin 6 */
    PIN_7 = (uint16_t)0x0007,  /* GPIO pin 7 */
    PIN_8 = (uint16_t)0x0008,  /* GPIO pin 8 */
    PIN_9 = (uint16_t)0x0009,  /* GPIO pin 9 */
    PIN_10 = (uint16_t)0x000A, /* GPIO pin 10 */
    PIN_11 = (uint16_t)0x000B, /* GPIO pin 11 */
    PIN_12 = (uint16_t)0x000C, /* GPIO pin 12 */
    PIN_13 = (uint16_t)0x000D, /* GPIO pin 13 */
    PIN_14 = (uint16_t)0x000E, /* GPIO pin 14 */
    PIN_15 = (uint16_t)0x000F, /* GPIO pin 15 */
    PIN_ALL = (uint16_t)0xffff /* Select all gpio pins */
};

/**
 * @brief GPIO logical pin state
 *
 */
enum PinState : uint8_t {
    LOW = (uint8_t)0x00, /* Pin state logical LOW */
    HIGH = (uint8_t)0x01 /* Pin state logical HIGH */
};

enum PinMode { INPUT = 0x00, OUTPUT_10, OUTPUT_2, OUTPUT_50 };

enum PinConfig {
    INPUT_ANALOG,
    INPUT_FLOATING,
    INPUT_PULLUP_PULLDOWN,
    RESERVED,
    OUTPUT_PUSH_PULL = 0x00,
    OUTPUT_OPEN_DRAIN = 0x01,
    ALT_PUSH_PULL = 0x03,
    ALT_OPEN_DRAIN = 0x04
};

private:
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
public:

/**
 * @brief
 * Initialize port and enable clock
 */
GPIO() {
    this->enableClock();
}

/**
 * @brief Overload placement new operator
 */
void *operator new(size_t, Port port) {
    return reinterpret_cast<void *>(port);
}

/**
 * @brief Enable clock for the instantiated port
 */
void enableClock() {
    if (this == reinterpret_cast<GPIO *>(PortA)) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    } else if (this == reinterpret_cast<GPIO *>(PortB)) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    } else if (this == reinterpret_cast<GPIO *>(PortC)) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    }
}

/**
 * @brief disable clock for the port to save power
 */
void disableClock() {
    if (this == reinterpret_cast<GPIO *>(PortA)) {
        RCC->APB2ENR &= ~RCC_APB2ENR_IOPAEN;
    } else if (this == reinterpret_cast<GPIO *>(PortB)) {
        RCC->APB2ENR &= ~RCC_APB2ENR_IOPBEN;
    } else if (this == reinterpret_cast<GPIO *>(PortC)) {
        RCC->APB2ENR &= ~RCC_APB2ENR_IOPCEN;
    }
}

void setPinMode(PIN pin, PinMode mode) {
    if (pin < PIN_8) {
        // clear previously set mode if any
        CRL &= ~((0x03) << (pin << 2));
        CRL |= mode << (pin << 2);
    } else {
        CRH &= ~(0x03 << ((pin - PIN_8) << 2));
        CRH |= mode << ((pin - PIN_8) << 2);
    }
}

void setPinConfig(PIN pin, PinConfig config) {
    if (pin < PIN_8) {
        // clear previously set mode if any
        CRL &= ~(0x06 << (pin << 2));
        CRL |= config << (pin << 2);
    } else {
        CRH &= ~(0x06 << ((pin - PIN_8) << 2));
        CRH |= (config << 2) << ((pin - PIN_8) << 2);
    }
}
/**
 * @brief Destroy the GPIO object
 *
 */
~GPIO() { this->disableClock(); }
};
#endif /* __GPIO_H__ */
