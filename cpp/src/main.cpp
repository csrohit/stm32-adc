/**
 * @file main.cpp
 * @author Rohit Nimkar (nehalnimkar@gmail.com) <https://csrohit.github.io>
 * @brief Main program body
 * @version 1.2
 *
 * @copyright Copyright (c) 2023
 * @attention
 *
 * This software component is licensed by Rohit Nimkar under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at: opensource.org/licenses/BSD-3-Clause
 *
 */

#include <stdint.h>

#include "printf.h"
#include "adc.h"
#include "gpio_port.h"
#include "stm32f1xx.h"
#include "usart.h"
#include "rcc.h"

char msg[28];

/**
 * @brief
 * Configure and initialize clock for SYSCLK = 72MHz
 */
void init_clock(void)
{
    FLASH->ACR |= FLASH_ACR_LATENCY_2;  // Two wait states, per datasheet
    RCC->CFGR |= RCC_CFGR_PPRE1_2;      // prescale AHB1 = HCLK/2
    RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE; // PREDIV1 = 0
    RCC->CR |= RCC_CR_HSEON;            // enable HSE clock
    while (!(RCC->CR & RCC_CR_HSERDY))
        ; // wait for the HSEREADY flag

    RCC->CFGR |= RCC_CFGR_PLLSRC;   // set PLL source to HSE
    RCC->CFGR |= RCC_CFGR_PLLMULL9; // multiply by 9
    RCC->CR |= RCC_CR_PLLON;        // enable the PLL
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ; // wait for the PLLRDY flag

    RCC->CFGR |= RCC_CFGR_SW_PLL; // set clock source to pll

    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL))
        ; // wait for PLL to be CLK

    Core::clock = 72000000U;
}

int main(void) {
    
    /* Initialize clock to 72MHz */
    init_clock();

    /* Configure PA0 in analog input mode*/
    GPIO &porta = *new (GPIO::PortA) GPIO;
    porta.setPinMode(GPIO::PIN_0, GPIO::INPUT);
    porta.setPinConfig(GPIO::PIN_0, GPIO::INPUT_ANALOG);

    /* Configure ADC1*/
    ADC &adc = *new (ADC::Adc1) ADC;
    Rcc::setADCPrescalar(Rcc::PCLK2_6);

    // wake from power down mode
    adc.setPowerState(ADC::Enabled);
    
    // set continuous conversion mode
    adc.setContinuousModeState(ADC::Enabled);

    // (optional) set data alignment in data register
    adc.setAlignment(ADC::ALIGN_RIGHT);
    
    /* Set sampling time = 28.5 cycles*/
    adc.setSamplingTime(ADC::Channel0, ADC::Cycles28_5);

    // recaloberate adc for more accurate results
    adc.resetCaliberation();
    adc.caliberate();

    // start ADc conversion
    adc.startConversion();

    /* Serial port configuration */
    USART &ttl = *new (USART::Usart1) USART;
    ttl.set_baudrate(USART::BR_115200);
    ttl.setTransmitterState(USART::Enabled);
    ttl.setUsartState(USART::Enabled);

    SysTick_Config(Core::clock / 1000);

    while (1) {
        uint16_t adc_value = adc.getData();
        sprintf_(msg, "Digital value: %hu\r\n", adc_value);
        ttl.tx_str(msg);
        delay_ms(1000);
    }
}
