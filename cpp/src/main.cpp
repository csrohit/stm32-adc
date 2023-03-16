/**
 ******************************************************************************
 * @file           : main.cpp
 * @author         : Rohit Nimkar <nehalnimkar@gmail.com>
 *<https://csrohit.github.io>
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * This software component is licensed by Rohit Nimkar under BSD 3-Clause
 *license, the "License"; You may not use this file except in compliance with
 *the License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "printf.h"
#include <adc.h>

#include <gpio.h>
#include <stdint.h>
#include <stm32f1xx.h>
#include "usart.h"
#include "rcc.h"

char msg[28];

volatile uint32_t msTicks = 0;

extern "C" {
void SysTick_Handler(void) { msTicks = msTicks + 1; }
}

/**
 * @brief Add blocking delay
 *
 * @param ms delay in milliseconds
 */
void delay(uint32_t ms) {
    uint32_t expected_ticks = msTicks + ms;
    while (msTicks < expected_ticks) {
        __asm("nop");
    }
}

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

    ADC &adc = *new (ADC::Adc1) ADC;

    Rcc::setADCPrescalar(Rcc::PCLK2_6);

    GPIO::enable_PortA();

    /* COnfigure PA0 in analog input mode */
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);

    // wake from power down mode
    adc.setPowerState(ADC::Enabled);
    adc.setContinuousModeState(ADC::Enabled);
    adc.setAlignment(ADC::ALIGN_RIGHT);
    
    /* Set sampling time = 28.5 cycles*/
    ADC1->SMPR2 |= (ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0);

    /* Reset Caliberation registers */
    ADC1->CR2 |= (ADC_CR2_RSTCAL);
    while (ADC1->CR2 & ADC_CR2_RSTCAL)
        ;

    /* Start caliberation */
    ADC1->CR2 |= (ADC_CR2_CAL);
    while (ADC1->CR2 & ADC_CR2_CAL)
        ;

    /* Start conversion */
    ADC1->CR2 |= ADC_CR2_ADON;

    USART &ttl = *new (USART::Usart1) USART;

    //
    /* Recaliberate ADC for better accuraccy */
    // adc.resetCaliberation();
    // adc.caliberate();
    // adc.caliberate();
    /* Set sampling time */

    /* Set gpio pin configuration */

    ttl.set_baudrate(USART::BR_115200);
    ttl.setTransmitterState(USART::Enabled);
    ttl.setUsartState(USART::Enabled);
    SysTick_Config(Core::clock / 1000);
    while (1) {
        uint16_t adc_value = ADC1->DR & 0xffff;
        sprintf_(msg, "Digital value: %hu\r\n", adc_value);
        ttl.tx_str(msg);
        delay(1000);
    }
}
