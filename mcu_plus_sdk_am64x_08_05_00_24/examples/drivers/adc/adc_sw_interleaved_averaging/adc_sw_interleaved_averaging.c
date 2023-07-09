/*
 *  Copyright (C) 2022 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/HwiP.h>
#include <drivers/adc.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"


/*
 * Example Description :
 *      This Example Demonstrates Software Interleaved Averaging of ADC input
 * channles. Channels 0 and 1 of ADC 1 and ADC 2 are sampled one after
 * ther other in order to achieve he interleaved averaging.
 *
 * Initially, a GPIO-->InputXbar[5] trigger is provided for the ADCs.
 * thereafter, the SOC/EOC15 will trigger ADCxINT1, hence triggering all SOCs.
 *
 * ISRs configured for ADC1_INT1 and ADC1_INT2, read results for SOCs
 * completed so far in the conversion cycle and average them.
 *
 * SOC Configurations :
 *      - even SOC (0,2,4,6,8,10,12,14) sample Channel 0
 *      - odd SOC (1,3,5,7,9,11,13,15) sample Channel 1
 *      - All SOC are triggered by INPUTXbar[5] and are also configured to be
 * triggered by ADCINT1 (generated by SOC/EOC15)
 *      - ADCx_INT1 triggers all the SOC in given ADCx
 *
 * Interrupt configurations :
 *      - ISR (App_adcISR_EOC15) is triggered by ADC1_INT1 (interrupt source is ADC1_SOC15)
 *      - ISR (App_adcISR_EOC7) is triggered by ADC1_INT2 (interrupt source is ADC1_SOC7)
 *      - ADC2_INT1 interrupt source is ADC2_SOC15
 *      - ADCxINTy are configured as Early interrupts (end of acquisition period)
 *
 * External Connections :
 *      - CC :
 *          - input trigger pulse on GPIO 7 (configured as input) - HSEC connecter pin - 80
 *            or connect it to GPIO 8 (Configured as output) - HSEC Connecter pin - 82
 *          - feed analog input on ADC 1 Channel 0 - HSEC connecter pin - 18
 *          - feed analog input on ADC 1 Channel 1 - HSEC connecter pin - 20
 *          - feed analog input on ADC 2 Channel 0 - HSEC connecter pin - 24
 *          - feed analog input on ADC 2 Channel 1 - HSEC connecter pin - 26
 *      - LP :
 *          - input trigger pulse on GPIO 16 (configured as input) - J5/J7 pin - 47
 *            or connect it to GPIO 15 (Configured as output) -  J6/J8 Jpin - 58
 *          - feed analog input on ADC 1 Channel 0 - J1/J3 Pin - 24
 *          - feed analog input on ADC 1 Channel 1 - J1/J3 Pin - 29
 *          - feed analog input on ADC 2 Channel 0 - J1/J3 Pin - 25
 *          - feed analog input on ADC 2 Channel 1 - J5/J7 Pin - 63
 *
 * Watch variables:
 *      - gAdc1Channel0Results - array of averaged Analog to Digital Converted samples on
 *                               channel 0 of ADC 1
 *      - gAdc1Channel1Results - array of averaged Analog to Digital Converted samples on
 *                               channel 1 of ADC 1
 *      - gAdc2Channel0Results - array of averaged Analog to Digital Converted samples on
 *                               channel 0 of ADC 2
 *      - gAdc2Channel1Results - array of averaged Analog to Digital Converted samples on
 *                               channel 1 of ADC 2
 */


/* Macros and global variables */
#define ADC_CONVERSION_COUNT        256

uint32_t gAdc1BaseAddr = CONFIG_ADC1_BASE_ADDR;
uint32_t gAdc2BaseAddr = CONFIG_ADC2_BASE_ADDR;
uint32_t gAdc1ResultBaseAddr = CONFIG_ADC1_RESULT_BASE_ADDR;
uint32_t gAdc2ResultBaseAddr = CONFIG_ADC2_RESULT_BASE_ADDR;


volatile uint16_t gAdc1Channel0Results[ADC_CONVERSION_COUNT];
volatile uint16_t gAdc1Channel1Results[ADC_CONVERSION_COUNT];
volatile uint16_t gAdc2Channel0Results[ADC_CONVERSION_COUNT];
volatile uint16_t gAdc2Channel1Results[ADC_CONVERSION_COUNT];

volatile int32_t gAdcConversionCount;
volatile uint32_t gIndex;
volatile uint32_t sumAdc1Ch0 = 0,   sumAdc1Ch1 = 0, sumAdc2Ch0 = 0,   sumAdc2Ch1 = 0;

static HwiP_Object gAdcHwiObject1;
static HwiP_Object gAdcHwiObject2;

static void App_adcISR_EOC7(void *args);
static void App_adcISR_EOC15(void *args);

void adc_sw_interleaved_averaging_main(void *args)
{   gAdcConversionCount = ADC_CONVERSION_COUNT;
    Drivers_open();
    Board_driversOpen();
    DebugP_log("ADC Software Interleaved Averaging Test Started ...\r\n");
    for(gIndex = 0; gIndex < ADC_CONVERSION_COUNT; gIndex++)
    {
        gAdc1Channel0Results[gIndex] = 0;
        gAdc1Channel1Results[gIndex] = 0;
        gAdc2Channel0Results[gIndex] = 0;
        gAdc2Channel1Results[gIndex] = 0;
    }
    /* registering ISRs */
    int32_t     status;
    HwiP_Params hwiPrms;

    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum      = CSLR_R5FSS0_CORE0_CONTROLSS_INTRXBAR0_OUT_0;
    hwiPrms.callback    = &App_adcISR_EOC7;
    hwiPrms.isFIQ       = 1;
    status              = HwiP_construct(&gAdcHwiObject1, &hwiPrms);
    DebugP_assert(status == SystemP_SUCCESS);

    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum      = CSLR_R5FSS0_CORE0_CONTROLSS_INTRXBAR0_OUT_1;
    hwiPrms.callback    = &App_adcISR_EOC15;
    hwiPrms.isFIQ       = 1;
    status              = HwiP_construct(&gAdcHwiObject2, &hwiPrms);
    DebugP_assert(status == SystemP_SUCCESS);

    gIndex = 0;

    /*Clearing any previous interrupt flags*/
    ADC_clearInterruptStatus(gAdc1BaseAddr, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(gAdc1BaseAddr, ADC_INT_NUMBER2);
    if(ADC_getInterruptOverflowStatus(gAdc1BaseAddr, ADC_INT_NUMBER1))
    {
        ADC_clearInterruptOverflowStatus(gAdc1BaseAddr, ADC_INT_NUMBER1);
    }
    if(ADC_getInterruptOverflowStatus(gAdc1BaseAddr, ADC_INT_NUMBER2))
    {
        ADC_clearInterruptOverflowStatus(gAdc1BaseAddr, ADC_INT_NUMBER2);
    }

    /* toggling the GPIO 8, used for loopback*/
    GPIO_setDirMode(CONFIG_GPIO_INPUT_BASE_ADDR, CONFIG_GPIO_INPUT_PIN, CONFIG_GPIO_INPUT_DIR);
    GPIO_setDirMode(CONFIG_GPIO_OUTPUT_BASE_ADDR, CONFIG_GPIO_OUTPUT_PIN, CONFIG_GPIO_OUTPUT_DIR);
    GPIO_pinWriteHigh(CONFIG_GPIO_OUTPUT_BASE_ADDR, CONFIG_GPIO_OUTPUT_PIN);
    ClockP_usleep(100);
    GPIO_pinWriteLow(CONFIG_GPIO_OUTPUT_BASE_ADDR, CONFIG_GPIO_OUTPUT_PIN);

    /* waiting until the conversions are complete*/
    while(gAdcConversionCount > 0);

    /* printing the ADCs results*/
    uint32_t skipInterations = 32;
    DebugP_log("ADC-1-Channel-0\tADC-1-Channel-1\tADC-2-Channel-0\tADC-2-Channel-1\r\n");
    for(gIndex = 0; gIndex < ADC_CONVERSION_COUNT; gIndex += skipInterations)
    {
        DebugP_log("%d\t\t%d\t\t%d\t\t%d\r\n",
        gAdc1Channel0Results[gIndex],
        gAdc1Channel1Results[gIndex],
        gAdc2Channel0Results[gIndex],
        gAdc2Channel1Results[gIndex]);
    }

    HwiP_destruct(&gAdcHwiObject1);
    HwiP_destruct(&gAdcHwiObject2);

    DebugP_log("ADC Software Interleaved Averaging Test Passed\r\n");
    DebugP_log("All tests have passed!!\r\n");

    Board_driversClose();
    Drivers_close();
}

static void App_adcISR_EOC7(void *args)
{
    sumAdc1Ch0 = 0;
    sumAdc1Ch1 = 0;
    sumAdc2Ch0 = 0;
    sumAdc2Ch1 = 0;
    /* ISR for reading the results of SOC 0-7*/
    for(ADC_SOCNumber soc_number = ADC_SOC_NUMBER0;
                      soc_number < ADC_SOC_NUMBER7;
                      soc_number = soc_number + 2)
    {
        sumAdc1Ch0 += ADC_readResult(gAdc1ResultBaseAddr, soc_number);
        sumAdc1Ch1 += ADC_readResult(gAdc1ResultBaseAddr, soc_number + 1);
        sumAdc2Ch0 += ADC_readResult(gAdc2ResultBaseAddr, soc_number);
        sumAdc2Ch1 += ADC_readResult(gAdc2ResultBaseAddr, soc_number + 1);
    }
    ADC_clearInterruptStatus(gAdc1BaseAddr, ADC_INT_NUMBER2);
    if(ADC_getInterruptOverflowStatus(gAdc1BaseAddr, ADC_INT_NUMBER2))
    {
        ADC_clearInterruptOverflowStatus(gAdc1BaseAddr, ADC_INT_NUMBER2);
    }
}

static void App_adcISR_EOC15(void *args)
{   if(gAdcConversionCount > 0)gAdcConversionCount--;
    else
    {
        ADC_disableInterrupt(gAdc1BaseAddr, ADC_INT_NUMBER1);
        ADC_disableInterrupt(gAdc1BaseAddr, ADC_INT_NUMBER2);
    }

    ADC_clearInterruptStatus(gAdc1BaseAddr, ADC_INT_NUMBER1);
    if(ADC_getInterruptOverflowStatus(gAdc1BaseAddr, ADC_INT_NUMBER1))
    {
        ADC_clearInterruptOverflowStatus(gAdc1BaseAddr, ADC_INT_NUMBER1);
    }
    /* ISR for reading the results of SOC 8-15*/
    for(ADC_SOCNumber soc_number = ADC_SOC_NUMBER8;
                      soc_number < ADC_SOC_NUMBER15;
                      soc_number = soc_number + 2)
    {
        sumAdc1Ch0 += ADC_readResult(gAdc1ResultBaseAddr, soc_number);
        sumAdc1Ch1 += ADC_readResult(gAdc1ResultBaseAddr, soc_number + 1);
        sumAdc2Ch0 += ADC_readResult(gAdc2ResultBaseAddr, soc_number);
        sumAdc2Ch1 += ADC_readResult(gAdc2ResultBaseAddr, soc_number + 1);
    }
    /* Averaging 8 results */
    /* Note : 8 results are 12 bit each. so maximum sum would be 7FF8, i.e., under 16 bits*/
    gAdc1Channel0Results[gIndex] = sumAdc1Ch0>>3;
    gAdc1Channel1Results[gIndex] = sumAdc1Ch1>>3;
    gAdc2Channel0Results[gIndex] = sumAdc2Ch0>>3;
    gAdc2Channel1Results[gIndex] = sumAdc2Ch1>>3;
    if(gIndex < ADC_CONVERSION_COUNT-1)
    {
        gIndex++;
    }
}