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

class ADC
{
private:
    volatile uint32_t SR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR1;
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;
    volatile uint32_t LTR;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;
    volatile uint32_t JDR1;
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;

public:
    typedef enum ADCInstance : uint32_t
    {
        Adc1 = ADC1_BASE
    } adc_t;

    typedef enum
    {
        Disabled,
        Enabled
    } ScanModeState,
        InterruptState, DMAState, ContModeState, PowerState;

    enum Alignment : uint32_t
    {
        ALIGN_RIGHT,
        ALIGN_LEFT
    };

    enum Channel : uint32_t
    {
        Channel0 = ADC_SMPR2_SMP0_Pos,
        Channel1 = ADC_SMPR2_SMP1_Pos,
        Channel2 = ADC_SMPR2_SMP2_Pos,
        Channel3 = ADC_SMPR2_SMP3_Pos,
        Channel4 = ADC_SMPR2_SMP4_Pos,
        Channel5 = ADC_SMPR2_SMP5_Pos,
        Channel6 = ADC_SMPR2_SMP6_Pos,
        Channel7 = ADC_SMPR2_SMP7_Pos,
        Channel8 = ADC_SMPR2_SMP8_Pos,
        Channel9 = ADC_SMPR2_SMP9_Pos,
        Channel10 = ADC_SMPR1_SMP10_Pos
    };

    enum SamplingTime : uint32_t
    {
        Cycles1_5 = 0x00,
        Cycles7_5,
        Cycles13_5,
        Cycles28_5,
        Cycles41_5,
        Cycles55_5,
        Cycles71_5,
        Cycles239_5,
    };

    ADC()
    {
        if (this == reinterpret_cast<ADC *>(Adc1))
        {
            // enable adc clock
            RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
        }
    }

    /* overload placement new operator */
    void *operator new(size_t, adc_t adc)
    {
        return reinterpret_cast<void *>(adc);
    }

    void setScanModeState(ScanModeState state)
    {
        if (ScanModeState::Enabled == state)
        {
            CR1 |= ADC_CR1_SCAN;
        }
        else
        {
            CR1 &= ~ADC_CR1_SCAN;
        }
    }

    void setEOCInterruptState(InterruptState state)
    {
        if (InterruptState::Enabled == state)
        {
            CR1 |= ADC_CR1_AWDIE;
        }
        else
        {
            CR1 &= ~ADC_CR1_AWDIE;
        }
    }

    void setAWDInterruptState(InterruptState state)
    {
        if (InterruptState::Enabled == state)
        {
            CR1 |= ADC_CR1_EOCIE;
        }
        else
        {
            CR1 &= ~ADC_CR1_EOCIE;
        }
    }

    void setContinuousModeState(ContModeState state)
    {
        if (ContModeState::Enabled == state)
        {
            CR2 |= ADC_CR2_CONT;
        }
        else
        {
            CR2 &= ~ADC_CR2_CONT;
        }
    }

    void setDMAState(DMAState state)
    {
        if (DMAState::Enabled == state)
        {
            CR2 |= ADC_CR2_DMA;
        }
        else
        {
            CR2 &= ~ADC_CR2_DMA;
        }
    }

    void setPowerState(PowerState state)
    {
        if (PowerState::Enabled == state)
        {
            CR2 |= ADC_CR2_ADON;
        }
        else
        {
            CR2 &= ~ADC_CR2_ADON;
        }
    }

    void startConversion()
    {
        setPowerState(PowerState::Enabled);
    }

    void resetCaliberation()
    {
        CR2 |= ADC_CR2_RSTCAL;
        while (CR2 & ADC_CR2_RSTCAL)
            ;
    }

    void caliberate()
    {
        CR2 |= ADC_CR2_CAL;
        while (CR2 & ADC_CR2_CAL)
            ;
    }

    void setAlignment(Alignment align)
    {
        if (Alignment::ALIGN_RIGHT == align)
        {
            CR2 &= ~ADC_CR2_ALIGN;
        }
        else
        {
            CR2 |= ADC_CR2_ALIGN;
        }
    }

    Alignment getAlignment(){
        if(CR2 & ADC_CR2_ALIGN){
            return ALIGN_LEFT;
        }else{
            return ALIGN_RIGHT;
        }
    }

    /**
     * @brief Set the Sampling Time for selected channel
     * @param channel selected channel
     * @param samplingTime required sampling time
     */
    void setSamplingTime(Channel channel, SamplingTime samplingTime)
    {
        if (channel < Channel10)
        {
            SMPR2 &= ~(0x7U << channel); // clear older selection
            SMPR2 |= (0x07 & samplingTime) << channel;
        }
    }

    /**
     * @brief Read digital value after conversion
     * @return uint16_t 12 bit converted digital value
     */
    uint16_t getData(){
        if(getAlignment() == ALIGN_RIGHT){
            return (uint16_t)(DR & 0xfff);
        }else{
            return (uint16_t)(DR & 0xfff0);
        }
    }

private:
};

#endif /* __ADC_H__ */
