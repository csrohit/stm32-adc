#include <stdint.h>
#include <stddef.h>

#include "stm32f1xx.h" 

#ifndef RCC_H
#define RCC_H

class Rcc {
private:
  volatile uint32_t CR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t APB2RSTR;
  volatile uint32_t APB1RSTR;
  volatile uint32_t AHBENR;
  volatile uint32_t APB2ENR;
  volatile uint32_t APB1ENR;
  volatile uint32_t BDCR;
  volatile uint32_t CSR;

public:

    enum AdcPrescalar{
        PCLK2_2 = RCC_CFGR_ADCPRE_DIV2,
        PCLK2_4 = RCC_CFGR_ADCPRE_DIV4,
        PCLK2_6 = RCC_CFGR_ADCPRE_DIV6,
        PCLK2_8 = RCC_CFGR_ADCPRE_DIV8,
    };
    
    /**
    * @brief Set Adc clock prescalar
    */
    static inline void setADCPrescalar(AdcPrescalar prescalar){
       RCC->CFGR |= prescalar; 
    }

};

#endif // RCC_H
