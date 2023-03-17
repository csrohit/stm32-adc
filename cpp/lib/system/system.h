/**
 * @file system.h
 * @author Rohit Nimkar (nehalnimkar@gmail.com) <https://csrohit.github.io>
 * @brief Declaration of functionality related to global functions
 * @version 1.2
 *
 * @copyright Copyright (c) 2022
 * @attention
 *
 * This software component is licensed by Rohit Nimkar under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at: opensource.org/licenses/BSD-3-Clause
 */

#include <stdint.h>

#include "stm32f1xx.h"

#ifndef SYSTEM_H
#define SYSTEM_H

namespace Core
{
    extern uint32_t clock;
    extern volatile uint32_t ms_ticks;
} // namespace Core

static inline void delay_ms(uint32_t ms)
{
    uint64_t final = ms + Core::ms_ticks;
    while (Core::ms_ticks < final)
    {
        __asm("nop");
    }
}

#endif //  SYSTEM_H

