/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Rohit Nimkar <nehalnimkar@gmail.com> <https://csrohit.github.io>
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * This software component is licensed by Rohit Nimkar under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <printf.h>
#include <stm32f1xx.h>

#include <timer.h>
#include <uart.h>
uint32_t SystemCoreClock = 8000000U;
char msg[28];

void init_clock(void)
{
	// Conf clock : 72MHz using HSE 8MHz crystal w/ PLL X 9 (8MHz x 9 = 72MHz)
	FLASH->ACR |= FLASH_ACR_LATENCY_2; // Two wait states, per datasheet
	RCC->CFGR |= RCC_CFGR_PPRE1_2;	   // prescale AHB1 = HCLK/2
	RCC->CR |= RCC_CR_HSEON;		   // enable HSE clock
	while (!(RCC->CR & RCC_CR_HSERDY))
		; // wait for the HSEREADY flag

	RCC->CFGR |= RCC_CFGR_PLLSRC;	// set PLL source to HSE
	RCC->CFGR |= RCC_CFGR_PLLMULL9; // multiply by 9
	RCC->CR |= RCC_CR_PLLON;		// enable the PLL
	while (!(RCC->CR & RCC_CR_PLLRDY))
		; // wait for the PLLRDY flag

	RCC->CFGR |= RCC_CFGR_SW_PLL; // set clock source to pll

	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL))
		; // wait for PLL to be CLK

	SystemCoreClock = 72000000;
}

void init_adc(void){
	// ADC clock should be <= 14MHz so divided PCLK2 (72MHz) by 6
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;	// adc_clk = 12MHz
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN;

	// set PA0 to analog input mode
	GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);

    // set sample time for ch 0 to 28.5 cycles (0b011)
    ADC1->SMPR2 |= ( ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0);
    
    // ADC1->SQR1 L[3:0] = 0b0000 at reset; set for 1 conversion
    // ADC1->SQR3 SQ1[4:0] = 0b00000 at reset; 1st conversion = chan 0
    
    // put ADC1 into continuous mode and turn on ADC
    ADC1->CR2 |= (ADC_CR2_CONT | ADC_CR2_ADON);

	ADC1->CR2 &= ~ADC_CR2_ALIGN;	// right alignment
    
    // reset calibration registers
    ADC1->CR2 |= (ADC_CR2_RSTCAL);
    while(ADC1->CR2 & ADC_CR2_RSTCAL);
    
    // enable calibration
    ADC1->CR2 |= (ADC_CR2_CAL);
    while(ADC1->CR2 & ADC_CR2_CAL);
    
    // not concerned about power consumption, just start the continuous
    // conversions and will read the DR periodically
    
    // start conversions
    ADC1->CR2 |= ADC_CR2_ADON;
}

int main(void)
{
	init_clock();
	init_adc();
	USART1_init(9600U);

	int ret = SysTick_Config(SystemCoreClock / 1000);
	if (ret < 0)
		while (1)
			;

	while (1)
	{
		uint16_t adc_value = ADC1->DR;
		sprintf_(msg, "Digital value: %hu\r\n", adc_value);
		USART1_puts(msg);
		delay(10U);
	}
}
