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
/*
 * Auto generated file
 */
#include <stdio.h>
#include <drivers/soc.h>
#include <kernel/dpl/AddrTranslateP.h>
#include "ti_dpl_config.h"
#include "ti_drivers_config.h"

/* ----------- HwiP ----------- */
HwiP_Config gHwiConfig = {
    .intcBaseAddr = 0x2FFF0000u,
};

/* ----------- ClockP ----------- */
#define TIMER11_CLOCK_SRC_MUX_ADDR (0x430081DCu)
#define TIMER11_CLOCK_SRC_MCU_HFOSC0 (0x0u)
#define TIMER11_BASE_ADDR     (0x24B0000u)

ClockP_Config gClockConfig = {
    .timerBaseAddr = TIMER11_BASE_ADDR, 
    .timerHwiIntNum = 163,
    .timerInputClkHz = 25000000,
    .timerInputPreScaler = 1,
    .usecPerTick = 1000,
};

/* ----------- DebugP ----------- */
void putchar_(char character)
{
    /* Output to memory trace buffer */
    DebugP_memLogWriterPutChar(character);
    /* Output to shared memory */
    DebugP_shmLogWriterPutChar(character);
}

/* DebugP log buffer memory and size
 * - This log can be viewed via ROV in CCS
 * - When linux is enabled, this log can also be viewed via linux debugfs
 */
char gDebugMemLog[DebugP_MEM_LOG_SIZE] __attribute__ ((section (".bss.debug_mem_trace_buf"), aligned (128)));
uint32_t gDebugMemLogSize = DebugP_MEM_LOG_SIZE;

/* Shared memory log base address, logs of each CPUs are put one after other in the below region.
 *
 * IMPORTANT: Make sure of below,
 * - The section defined below should be placed at the exact same location in memory for all the CPUs
 * - The memory should be marked as non-cached for all the CPUs
 * - The section should be marked as NOLOAD in all the CPUs linker command file
 */
DebugP_ShmLog gDebugShmLog[CSL_CORE_ID_MAX] __attribute__((aligned(128), section(".bss.log_shared_mem")));

/* ----------- CacheP ----------- */
const CacheP_Config gCacheConfig = {
    .enable = 1,
    .enableForceWrThru = 0,
};

/* ----------- MpuP_armv7 ----------- */
#define CONFIG_MPU_NUM_REGIONS  (7u)

const MpuP_Config gMpuConfig = {
    .numRegions = CONFIG_MPU_NUM_REGIONS,
    .enableBackgroundRegion = 0,
    .enableMpu = 1,
};

const MpuP_RegionConfig gMpuRegionConfig[CONFIG_MPU_NUM_REGIONS] =
{
    {
        .baseAddr = 0x0u,
        .size = MpuP_RegionSize_2G,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 0,
            .isBufferable = 0,
            .isSharable = 1,
            .isExecuteNever = 1,
            .tex = 0,
            .accessPerm = MpuP_AP_S_RW_U_R,
            .subregionDisableMask = 0x0u
        },
    },
    {
        .baseAddr = 0x0u,
        .size = MpuP_RegionSize_32K,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 1,
            .isBufferable = 1,
            .isSharable = 0,
            .isExecuteNever = 0,
            .tex = 1,
            .accessPerm = MpuP_AP_S_RW_U_R,
            .subregionDisableMask = 0x0u
        },
    },
    {
        .baseAddr = 0x41010000u,
        .size = MpuP_RegionSize_32K,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 1,
            .isBufferable = 1,
            .isSharable = 0,
            .isExecuteNever = 0,
            .tex = 1,
            .accessPerm = MpuP_AP_S_RW_U_R,
            .subregionDisableMask = 0x0u
        },
    },
    {
        .baseAddr = 0x70000000u,
        .size = MpuP_RegionSize_2M,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 1,
            .isBufferable = 1,
            .isSharable = 0,
            .isExecuteNever = 0,
            .tex = 1,
            .accessPerm = MpuP_AP_S_RW_U_R,
            .subregionDisableMask = 0x0u
        },
    },
    {
        .baseAddr = 0xA5000000u,
        .size = MpuP_RegionSize_64K,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 0,
            .isBufferable = 0,
            .isSharable = 1,
            .isExecuteNever = 1,
            .tex = 1,
            .accessPerm = MpuP_AP_ALL_RW,
            .subregionDisableMask = 0x0u
        },
    },
    {
        .baseAddr = 0x80000000u,
        .size = MpuP_RegionSize_2G,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 1,
            .isBufferable = 1,
            .isSharable = 0,
            .isExecuteNever = 0,
            .tex = 1,
            .accessPerm = MpuP_AP_ALL_RW,
            .subregionDisableMask = 0x0u
        },
    },
    {
        .baseAddr = 0xA3000000u,
        .size = MpuP_RegionSize_1M,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 0,
            .isBufferable = 0,
            .isSharable = 1,
            .isExecuteNever = 1,
            .tex = 1,
            .accessPerm = MpuP_AP_ALL_RW,
            .subregionDisableMask = 0x0u
        },
    },
};


#define BOOT_SECTION __attribute__((section(".text.boot")))

/* This function is called by _c_int00 */
void BOOT_SECTION __mpu_init() 
{
    MpuP_init();
    
    CacheP_init();
}

void Dpl_init(void)
{
    /* initialize Hwi but keep interrupts disabled */
    HwiP_init();

    /* init debug log zones early */
    /* Debug log init */
    DebugP_logZoneEnable(DebugP_LOG_ZONE_ERROR);
    DebugP_logZoneEnable(DebugP_LOG_ZONE_WARN);
    /* Initialize shared memory writer on this CPU */
    DebugP_shmLogWriterInit(&gDebugShmLog[CSL_CORE_ID_R5FSS1_1], CSL_CORE_ID_R5FSS1_1);
    /* Initialize linux trace log writer */
    DebugP_memLogWriterInit(CSL_CORE_ID_R5FSS1_1);


    /* set timer clock source */
    SOC_controlModuleUnlockMMR(SOC_DOMAIN_ID_MAIN, 2);
    *(volatile uint32_t*)(TIMER11_CLOCK_SRC_MUX_ADDR) = TIMER11_CLOCK_SRC_MCU_HFOSC0;
    SOC_controlModuleLockMMR(SOC_DOMAIN_ID_MAIN, 2);
    /* initialize Clock */
    ClockP_init();



    /* Enable interrupt handling */
    HwiP_enable();
}

void Dpl_deinit(void)
{
}
