/*
 *  Copyright (C) 2018-2022 Texas Instruments Incorporated
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

#include <kernel/dpl/AddrTranslateP.h>
#include <drivers/soc.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

/*
 * This example configures a GPIO pin in input mode
 * and configures it to generate interrupt on rising edge.
 * The application waits for 5 key presses, prints the
 * number of times the keys are pressed and exits.
 */

uint32_t            gGpioBaseAddr = GPIO_PUSH_BUTTON_BASE_ADDR;
HwiP_Object         gGpioHwiObject;

extern void Board_gpioInit(void);
extern void Board_gpioDeinit(void);
extern uint32_t Board_getGpioButtonIntrNum(void);
extern uint32_t Board_getGpioButtonSwitchNum(void);

static void GPIO_bankIsrFxn(void *args)
{
    uint32_t    pinNum = (uint32_t) args;
    uint32_t    bankNum =  GPIO_GET_BANK_INDEX(pinNum);
    uint32_t    intrStatus, pinMask = GPIO_GET_BANK_BIT_MASK(pinNum);
    /* Get and clear bank interrupt status */
    intrStatus = GPIO_getBankIntrStatus(gGpioBaseAddr, bankNum);
    GPIO_clearBankIntrStatus(gGpioBaseAddr, bankNum, intrStatus);

    /* Per pin interrupt handling */
    if(intrStatus & pinMask)
    {
        SOC_generateSwWarmResetMainDomain ();
    }
}

void reset_isolation_main(void *args)
{
    int32_t         retVal;
    uint32_t        pinNum, intrNum;
    uint32_t        bankNum;
    HwiP_Params     hwiPrms;

    /* Open drivers to open the UART driver for console */
    Drivers_open();
    Board_driversOpen();

    Board_gpioInit();

    pinNum          = GPIO_PUSH_BUTTON_PIN;
    intrNum         = Board_getGpioButtonIntrNum();
    bankNum         = GPIO_GET_BANK_INDEX(pinNum);

    /* Address translate */
    gGpioBaseAddr = (uint32_t) AddrTranslateP_getLocalAddr(gGpioBaseAddr);

    /* Setup GPIO for interrupt generation */
    GPIO_setDirMode(gGpioBaseAddr, pinNum, GPIO_PUSH_BUTTON_DIR);
    GPIO_setTrigType(gGpioBaseAddr, pinNum, GPIO_PUSH_BUTTON_TRIG_TYPE);
    GPIO_bankIntrEnable(gGpioBaseAddr, bankNum);

    /* Register pin interrupt */
    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum   = intrNum;
    hwiPrms.callback = &GPIO_bankIsrFxn;
    hwiPrms.args     = (void *) pinNum;
    retVal = HwiP_construct(&gGpioHwiObject, &hwiPrms);
    DebugP_assert(retVal == SystemP_SUCCESS );

    DebugP_log ("Starting R5 \r\n");
    DebugP_log("Press and release SW%d button on EVM to trigger warm reset from SW...\r\n", Board_getGpioButtonSwitchNum());
    DebugP_log("Press and release SW4 button on EVM to trigger a warm reset from HW..\r\n");

    DebugP_log("\r\n");

    int count = 0;
    while(1)
    {
        /* Print a hearbeat message to indicate the core is running */
        ClockP_sleep(1);

        DebugP_log("I am running (R5) !!:- %d\r\n", count++);
    }

    Board_gpioDeinit();
    Board_driversClose();
    Drivers_close();
}
