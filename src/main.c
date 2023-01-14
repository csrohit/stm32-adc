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

int main(void)
{

	init_clock();

	USART1_init(9600U);

	int ret = SysTick_Config(SystemCoreClock / 1000);
	if (ret < 0)
		while (1)
			;

	while (1)
	{
		sprintf_(msg, "Time elapsed: %lu ms\r\n", msTicks);
		USART1_puts(msg);
		delay(5000U);
	}
}
