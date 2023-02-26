/**
 ******************************************************************************
 * @file           : main.cpp
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

#include <stm32f1xx.h>
#include <stdint.h>
#include <gpio.h>
#include <usart.h>

const char * msg = "Hello World\r\n\0";
volatile uint32_t msTicks = 0;
extern "C"
{
	void SysTick_Handler(void)
	{
		msTicks++;
	}
}

/**
 * @brief Add blocking delay
 *
 * @param ms delay in milliseconds
 */
void delay(int ms)
{
	uint32_t expected_ticks = msTicks + ms;
	while (msTicks < expected_ticks)
	{
		__asm("nop");
	}
}

int main(void)
{
	USART  &ttl = *new (USART::Usart1) USART;

	ttl.set_baudrate(USART::BR_115200);
	ttl.setTransmitterState(USART::Enabled);
	ttl.setUsartState(USART::Enabled);
	SysTick_Config(SystemCoreClock / 1000);

	GPIO::enable_PortC();
	GPIO led(GPIOC, GPIO::PIN_13);
	while (1)
	{
		led.toggle();
		delay(1000);
		ttl.tx_str(msg);
	}
}