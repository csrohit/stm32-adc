/**
 * @file adc.h
 * @author Rohit Nimkar (nehalnimkar@gmail.com)
 * @brief Declaration of functionality related to adc
 * @version 1.2
 * @date 2022-12-07
 *
 * @copyright Copyright (c) 2022
 * @attention
 *
 * This software component is licensed by Rohit Nimkar under BSD 3-Clause
 * license, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 * opensource.org/licenses/BSD-3-Clause
 *
 */

#include "stm32f103xb.h"
#include <stddef.h>
#include <stm32f1xx.h>

#ifndef __ADC_H__
#define __ADC_H__

class ADC {
private:
  volatile uint32_t SR;
  volatile uint32_t CR1;
  volatile uint32_t CR2;

public:
  typedef enum ADCInstance : uint32_t { Adc1 = ADC1_BASE } adc_t;
  
  typedef enum
    {
      Disabled,
      Enabled
  } ScanModeState, InterruptState, DMAState, ContModeState, PowerState;

  enum Alignment : uint32_t { ALIGN_RIGHT, ALIGN_LEFT };

  ADC() {
    if (this == reinterpret_cast<ADC *>(Adc1)) {
      // enable adc clock
      RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    }
  }

  /* overload placement new operator */
  void *operator new(size_t, adc_t adc){
    return reinterpret_cast<void *>(adc);
  }


  void setScanModeState(ScanModeState state){
    if(ScanModeState::Enabled == state){
      CR1 |= ADC_CR1_SCAN;
    }else{ 
      CR1 &= ~ADC_CR1_SCAN;
    }
  }

  void setEOCInterruptState(InterruptState state){
    if(InterruptState::Enabled == state){
      CR1 |= ADC_CR1_AWDIE;
    }else{
      CR1 &= ~ADC_CR1_AWDIE;
    }
  }


  void setAWDInterruptState(InterruptState state){
    if(InterruptState::Enabled == state){
      CR1 |= ADC_CR1_EOCIE;
    }else{
      CR1 &= ~ADC_CR1_EOCIE;
    }
  }

  void setContinuousModeState(ContModeState state){
    if(ContModeState::Enabled == state){
      CR2 |= ADC_CR2_CONT;
    }else{
      CR2 &= ~ADC_CR2_CONT;
    }
  }

  void setDMAState(DMAState state){
    if(DMAState::Enabled == state){
      CR2 |= ADC_CR2_DMA;
    }else{
      CR2 &= ~ADC_CR2_DMA;
    }
  }


  void setPowerState(PowerState state){
    if(PowerState::Enabled == state){
      CR2 |= ADC_CR2_ADON;
    }else{
      CR2 &= ~ADC_CR2_ADON;
    }
  }


  void startConversion() { CR2 |= ADC_CR2_ADON; }

  void resetCaliberation() {
    CR2 |= ADC_CR2_RSTCAL;
    while (CR2 & ADC_CR2_RSTCAL)
      ;
  }

  void caliberate() {
    CR2 |= ADC_CR2_CAL;
    while (CR2 & ADC_CR2_CAL)
      ;
  }

  void setAlignment(Alignment align) {
    if (Alignment::ALIGN_RIGHT == align) {
      CR2 &= ~ADC_CR2_ALIGN;
    } else {
      CR2 |= ADC_CR2_ALIGN;
    }
  }

private:
};

#endif /* __ADC_H__ */
