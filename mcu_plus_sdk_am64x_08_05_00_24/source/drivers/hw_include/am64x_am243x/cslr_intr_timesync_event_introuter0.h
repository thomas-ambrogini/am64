/********************************************************************
*
* TIMESYNC_EVENT_INTROUTER0 INTERRUPT MAP. header file
*
* Copyright (C) 2015-2019 Texas Instruments Incorporated.
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
*
*/
#ifndef CSLR_TIMESYNC_EVENT_INTROUTER0_INTERRUPT_MAP_H_
#define CSLR_TIMESYNC_EVENT_INTROUTER0_INTERRUPT_MAP_H_

#include <drivers/hw_include/cslr.h>
#include <drivers/hw_include/tistdtypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
* List of intr sources for receiver: TIMESYNC_EVENT_INTROUTER0
*/

#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_TIMER0_TIMER_PWM_0                                       (0U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_TIMER1_TIMER_PWM_0                                       (1U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_TIMER2_TIMER_PWM_0                                       (2U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_TIMER3_TIMER_PWM_0                                       (3U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_PRG0_IEP0_EDC_LATCH_IN0IN_PRG0_IEP0_EDC_LATCH_IN0_0 (4U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_PRG0_IEP0_EDC_LATCH_IN1IN_PRG0_IEP0_EDC_LATCH_IN1_0 (5U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_PRG0_IEP1_EDC_LATCH_IN0IN_PRG0_IEP1_EDC_LATCH_IN0_0 (6U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_PRG0_IEP1_EDC_LATCH_IN1IN_PRG0_IEP1_EDC_LATCH_IN1_0 (7U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_PRG1_IEP0_EDC_LATCH_IN0IN_PRG1_IEP0_EDC_LATCH_IN0_0 (8U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_PRG1_IEP0_EDC_LATCH_IN1IN_PRG1_IEP0_EDC_LATCH_IN1_0 (9U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_PRG1_IEP1_EDC_LATCH_IN0IN_PRG1_IEP1_EDC_LATCH_IN0_0 (10U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_PRG1_IEP1_EDC_LATCH_IN1IN_PRG1_IEP1_EDC_LATCH_IN1_0 (11U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_CP_GEMAC_CPTS0_HW1TSPUSHIN_CP_GEMAC_CPTS0_HW1TSPUSH_0 (12U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_CP_GEMAC_CPTS0_HW2TSPUSHIN_CP_GEMAC_CPTS0_HW2TSPUSH_0 (13U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_CPTS0_HW1TSPUSHIN_CPTS0_HW1TSPUSH_0          (14U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PINFUNCTION_CPTS0_HW2TSPUSHIN_CPTS0_HW2TSPUSH_0          (15U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_CPTS0_CPTS_GENF0_0                                       (16U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_CPTS0_CPTS_GENF1_0                                       (17U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_CPTS0_CPTS_GENF2_0                                       (18U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_CPTS0_CPTS_GENF3_0                                       (19U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_CPTS0_CPTS_GENF4_0                                       (20U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_CPSW0_CPTS_GENF0_0                                       (21U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_CPSW0_CPTS_GENF1_0                                       (22U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PCIE0_PCIE_CPTS_GENF0_0                                  (23U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_CPTS0_CPTS_GENF5_0                                       (24U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PRU_ICSSG0_PR1_EDC0_SYNC0_OUT_0                          (25U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PRU_ICSSG0_PR1_EDC0_SYNC1_OUT_0                          (26U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PRU_ICSSG0_PR1_EDC1_SYNC0_OUT_0                          (27U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PRU_ICSSG0_PR1_EDC1_SYNC1_OUT_0                          (28U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PRU_ICSSG1_PR1_EDC0_SYNC0_OUT_0                          (29U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PRU_ICSSG1_PR1_EDC0_SYNC1_OUT_0                          (30U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PRU_ICSSG1_PR1_EDC1_SYNC0_OUT_0                          (31U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PRU_ICSSG1_PR1_EDC1_SYNC1_OUT_0                          (32U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PCIE0_PCIE_CPTS_SYNC_0                                   (33U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_CPSW0_CPTS_SYNC_0                                        (34U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_CPTS0_CPTS_SYNC_0                                        (35U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_GTC0_GTC_PUSH_EVENT_0                                    (36U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PCIE0_PCIE_CPTS_HW1_PUSH_0                               (37U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_PCIE0_PCIE_PTM_VALID_PULSE_0                             (38U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_EPWM0_EPWM_SYNCO_O_0                                     (39U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_EPWM3_EPWM_SYNCO_O_0                                     (40U)
#define CSLR_TIMESYNC_EVENT_INTROUTER0_IN_EPWM6_EPWM_SYNCO_O_0                                     (41U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_TIMESYNC_EVENT_INTROUTER0_INTERRUPT_MAP_H_ */
