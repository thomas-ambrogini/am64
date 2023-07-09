/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
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

#include "ti_board_open_close.h"

int32_t Board_driversOpen(void)
{
    int32_t status = SystemP_SUCCESS;
    if(status==SystemP_SUCCESS)
    {
        status = Board_eepromOpen();
    }

    if(status==SystemP_SUCCESS)
    {
        status = Board_flashOpen();
    }
    if(status==SystemP_SUCCESS)
    {
        status = Board_ledOpen();
    }

    if(status==SystemP_SUCCESS)
    {
        status = Board_ethPhyOpen();
    }

    return status;
}

void Board_driversClose(void)
{
    Board_eepromClose();

    Board_flashClose();
    Board_ledClose();

    Board_ethPhyClose();

}

/*
 * EEPROM
 */
#include <board/eeprom.h>
/* EEPROM specific includes */
#include <board/eeprom/eeprom_at24c.h>

/* EEPROM Object - initalized during EEPROM_open() */
EEPROM_Object gEepromObject_AT24C;

/* EEPROM Driver handles - opened during Board_eepromOpen() */
EEPROM_Handle gEepromHandle[CONFIG_EEPROM_NUM_INSTANCES];

/* EEPROM Config */
EEPROM_Config gEepromConfig[CONFIG_EEPROM_NUM_INSTANCES] =
{
    {
        .attrs = &gEepromAttrs_AT24C,
        .fxns = &gEepromFxns_AT24C,
        .object = (void *)&gEepromObject_AT24C,
    },
};
uint32_t gEepromConfigNum = CONFIG_EEPROM_NUM_INSTANCES;

/* EEPROM params */
EEPROM_Params gEepromParams[CONFIG_EEPROM_NUM_INSTANCES] =
{
    {
        .driverInstance = CONFIG_I2C0,
        .i2cAddress     = 0x50,
    },
};

int32_t Board_eepromOpen()
{
    uint32_t instCnt;
    int32_t  status = SystemP_SUCCESS;

    for(instCnt = 0U; instCnt < CONFIG_EEPROM_NUM_INSTANCES; instCnt++)
    {
        gEepromHandle[instCnt] = NULL;   /* Init to NULL so that we can exit gracefully */
    }

    /* Open all instances */
    for(instCnt = 0U; instCnt < CONFIG_EEPROM_NUM_INSTANCES; instCnt++)
    {
        gEepromHandle[instCnt] = EEPROM_open(instCnt, &gEepromParams[instCnt]);
        if(NULL == gEepromHandle[instCnt])
        {
            DebugP_logError("EEPROM open failed for instance %d !!!\r\n", instCnt);
            status = SystemP_FAILURE;
            break;
        }
    }

    if(SystemP_FAILURE == status)
    {
        Board_eepromClose();   /* Exit gracefully */
    }

    return status;
}

void Board_eepromClose(void)
{
    uint32_t instCnt;

    /* Close all instances that are open */
    for(instCnt = 0U; instCnt < CONFIG_EEPROM_NUM_INSTANCES; instCnt++)
    {
        if(gEepromHandle[instCnt] != NULL)
        {
            EEPROM_close(gEepromHandle[instCnt]);
            gEepromHandle[instCnt] = NULL;
        }
    }

    return;
}

/*
 * FLASH
 */
/* FLASH Object - initialized during Flash_open() */
Flash_NorOspiObject gFlashObject_S28HS512T;

/* FLASH Driver handles - opened during Board_flashOpen() */
Flash_Handle gFlashHandle[CONFIG_FLASH_NUM_INSTANCES];

/* FLASH Attrs */
Flash_Attrs gFlashAttrs_S28HS512T =
{
    .flashName = "S28HS512T",
    .deviceId = 0x5B1A,
    .manufacturerId = 0x34,
    .flashSize = 67108864,
    .blockCount = 256,
    .blockSize = 262144,
    .pageCount = 1024,
    .pageSize = 256,
    .sectorCount = 16384,
    .sectorSize = 4096,
};

/* FLASH DevConfig */
Flash_DevConfig gFlashDevCfg_S28HS512T =
{
    .cmdExtType = OSPI_CMD_EXT_TYPE_REPEAT,
    .enable4BAddr = TRUE,
    .addrByteSupport = 1,
    .fourByteAddrEnSeq = 0xA0,
    .cmdWren = 0x06,
    .cmdRdsr = 0x05,
    .srWip = (1 << 0),
    .srWel = (1 << 1),
    .xspiWipRdCmd = 0x65,
    .xspiWipReg = 0x00800000,
    .xspiWipBit = (1 << 0),
    .resetType = 0x10,
    .eraseCfg = {
        .blockSize = 262144,
        .sectorSize = 4096,
        .cmdBlockErase3B = 0xDC,
        .cmdBlockErase4B = 0xDC,
        .cmdSectorErase3B = 0x21,
        .cmdSectorErase4B = 0x21,
        .cmdChipErase = 0xC7,
    },
    .idCfg = {
        .cmd = 0x9F, /* Constant */
        .numBytes = 3,
        .dummy4 = 0,
        .dummy8 = 0,
    },
    .protocolCfg = {
        .protocol = FLASH_CFG_PROTO_8D_8D_8D,
        .isDtr = TRUE,
        .cmdRd = 0xEE,
        .cmdWr = 0x12,
        .modeClksCmd = 0,
        .modeClksRd = 0,
        .dummyClksCmd = 4,
        .dummyClksRd = 25,
        .enableType = 0,
        .enableSeq = 0x00,
        .protoCfg = {
            .isAddrReg = TRUE,
            .cmdRegRd = 0x65,
            .cmdRegWr = 0x71,
            .cfgReg = 0x00800006,
            .shift = 0,
            .mask = 0x01,
            .cfgRegBitP = 1,
        },
        .strDtrCfg = {
            .isAddrReg = TRUE,
            .cmdRegRd = 0x65,
            .cmdRegWr = 0x71,
            .cfgReg = 0x00800006,
            .shift = 1,
            .mask = 0x02,
            .cfgRegBitP = 1,
        },
        .dummyCfg = {
            .isAddrReg = TRUE,
            .cmdRegRd = 0x65,
            .cmdRegWr = 0x71,
            .cfgReg = 0x00800003,
            .shift = 0,
            .mask = 0x0F,
            .cfgRegBitP = 11,
        },
    },
    .flashWriteTimeout = 512,
    .flashBusyTimeout = 256000000,
};

/* FLASH Config */
Flash_Config gFlashConfig[CONFIG_FLASH_NUM_INSTANCES] =
{
    {
        .attrs = &gFlashAttrs_S28HS512T,
        .fxns = &gFlashNorOspiFxns,
        .devConfig = &gFlashDevCfg_S28HS512T,
        .object = (void *)&gFlashObject_S28HS512T,
    },
};

/* Externs of quirk functions and custom protocol functions */
extern int32_t Flash_quirkSpansionUNHYSADisable(Flash_Config *config);

/* FLASH Open Params - populated from SysConfig options */
Flash_Params gFlashParams[CONFIG_FLASH_NUM_INSTANCES] =
{
    {
        .quirksFxn = Flash_quirkSpansionUNHYSADisable,
        .custProtoFxn = NULL,
    },
};

uint32_t gFlashConfigNum = CONFIG_FLASH_NUM_INSTANCES;

int32_t Board_flashOpen()
{
    uint32_t instCnt;
    int32_t  status = SystemP_SUCCESS;

    /* Set the peripheral instance to use in the under lying flash implementation */
    gFlashConfig[CONFIG_FLASH0].attrs->driverInstance = CONFIG_OSPI0;

    for(instCnt = 0U; instCnt < CONFIG_FLASH_NUM_INSTANCES; instCnt++)
    {
        gFlashHandle[instCnt] = NULL;   /* Init to NULL so that we can exit gracefully */
    }

    /* Set the underlying driver instance to the FLASH config */
    gFlashAttrs_S28HS512T.driverInstance = CONFIG_OSPI0;

    /* Open all instances */
    for(instCnt = 0U; instCnt < CONFIG_FLASH_NUM_INSTANCES; instCnt++)
    {

        gFlashHandle[instCnt] = Flash_open(instCnt, &gFlashParams[instCnt]);
        if(NULL == gFlashHandle[instCnt])
        {
            DebugP_logError("FLASH open failed for instance %d !!!\r\n", instCnt);
            status = SystemP_FAILURE;
            break;
        }
    }

    if(SystemP_FAILURE == status)
    {
        Board_flashClose();   /* Exit gracefully */
    }
    return status;
}

void Board_flashClose(void)
{
    uint32_t instCnt;

    /* Close all instances that are open */
    for(instCnt = 0U; instCnt < CONFIG_FLASH_NUM_INSTANCES; instCnt++)
    {
        if(gFlashHandle[instCnt] != NULL)
        {
            Flash_close(gFlashHandle[instCnt]);
            gFlashHandle[instCnt] = NULL;
        }
    }
    return;
}

/*
 * LED
 */
/* LED specific includes */
#include <board/led/led_tpic2810.h>

/* LED Object - initalized during LED_open() */
LED_Object gLedObject[CONFIG_LED_NUM_INSTANCES];
/* LED Driver handles - opened during Board_ledOpen() */
LED_Handle gLedHandle[CONFIG_LED_NUM_INSTANCES];

/* LED Config */
LED_Config gLedConfig[CONFIG_LED_NUM_INSTANCES] =
{
    {
        .attrs = &gLedAttrs_TPIC2810,
        .fxns = &gLedFxns_TPIC2810,
        .object = (void *)&gLedObject[0],
    },
};
uint32_t gLedConfigNum = CONFIG_LED_NUM_INSTANCES;

/* LED params */
LED_Params gLedParams[CONFIG_LED_NUM_INSTANCES] =
{
    {
        .gpioBaseAddr   = 0U,
        .gpioPinNum     = 0U,
        .i2cInstance    = CONFIG_I2C1,
        .i2cAddress     = 0x60,
        .ioIndex        = 0U,
    },
};

int32_t Board_ledOpen()
{
    uint32_t instCnt;
    int32_t  status = SystemP_SUCCESS;

    for(instCnt = 0U; instCnt < CONFIG_LED_NUM_INSTANCES; instCnt++)
    {
        gLedHandle[instCnt] = NULL;   /* Init to NULL so that we can exit gracefully */
    }

    /* Open all instances */
    for(instCnt = 0U; instCnt < CONFIG_LED_NUM_INSTANCES; instCnt++)
    {
        gLedHandle[instCnt] = LED_open(instCnt, &gLedParams[instCnt]);
        if(NULL == gLedHandle[instCnt])
        {
            DebugP_logError("LED open failed for instance %d !!!\r\n", instCnt);
            status = SystemP_FAILURE;
            break;
        }
    }

    if(SystemP_FAILURE == status)
    {
        Board_ledClose();   /* Exit gracefully */
    }

    return status;
}

void Board_ledClose(void)
{
    uint32_t instCnt;

    /* Close all instances that are open */
    for(instCnt = 0U; instCnt < CONFIG_LED_NUM_INSTANCES; instCnt++)
    {
        if(gLedHandle[instCnt] != NULL)
        {
            LED_close(gLedHandle[instCnt]);
            gLedHandle[instCnt] = NULL;
        }
    }

    return;
}

/*
 * ETHPHY
 */
/* ETHPHY Driver handles - opened during Board_ethPhyOpen() */
ETHPHY_Handle gEthPhyHandle[CONFIG_ETHPHY_NUM_INSTANCES];

/* ETHPHY Attrs */
ETHPHY_Attrs gEthPhyAttrs[CONFIG_ETHPHY_NUM_INSTANCES] =
{
    {
        .mdioBaseAddress = CSL_PRU_ICSSG1_PR1_MDIO_V1P7_MDIO_BASE,
        .phyAddress      = 15,
    },
    {
        .mdioBaseAddress = CSL_PRU_ICSSG1_PR1_MDIO_V1P7_MDIO_BASE,
        .phyAddress      = 3,
    },
};


/* ETHPHY customer driver functions, MUST be implemented by users else there will be linker error */
extern ETHPHY_Fxns gEthPhyFxns_DP83869_0;
/* ETHPHY customer driver functions, MUST be implemented by users else there will be linker error */
extern ETHPHY_Fxns gEthPhyFxns_DP83869_1;

/* ETHPHY Config */
ETHPHY_Config gEthPhyConfig[CONFIG_ETHPHY_NUM_INSTANCES] =
{
    {
        .attrs = &gEthPhyAttrs[0],
        .fxns = &gEthPhyFxns_DP83869_0,
        .object = (void *)NULL,
    },
    {
        .attrs = &gEthPhyAttrs[1],
        .fxns = &gEthPhyFxns_DP83869_1,
        .object = (void *)NULL,
    },
};
uint32_t gEthPhyConfigNum = CONFIG_ETHPHY_NUM_INSTANCES;

ETHPHY_Params gEthPhyParams[CONFIG_ETHPHY_NUM_INSTANCES];

int32_t Board_ethPhyOpen()
{
    uint32_t instCnt;
    int32_t  status = SystemP_SUCCESS;

    for(instCnt = 0U; instCnt < CONFIG_ETHPHY_NUM_INSTANCES; instCnt++)
    {
        gEthPhyHandle[instCnt] = NULL;   /* Init to NULL so that we can exit gracefully */
    }

    /* Open all instances */
    for(instCnt = 0U; instCnt < CONFIG_ETHPHY_NUM_INSTANCES; instCnt++)
    {
        ETHPHY_Params_init(&gEthPhyParams[instCnt]);
        gEthPhyHandle[instCnt] = ETHPHY_open(instCnt, &gEthPhyParams[instCnt]);
        if(NULL == gEthPhyHandle[instCnt])
        {
            DebugP_logError("ETHPHY open failed for instance %d !!!\r\n", instCnt);
            status = SystemP_FAILURE;
            break;
        }
    }

    if(SystemP_FAILURE == status)
    {
        Board_ethPhyClose();   /* Exit gracefully */
    }
    return status;
}

void Board_ethPhyClose(void)
{
    uint32_t instCnt;

    /* Close all instances that are open */
    for(instCnt = 0U; instCnt < CONFIG_ETHPHY_NUM_INSTANCES; instCnt++)
    {
        if(gEthPhyHandle[instCnt] != NULL)
        {
            ETHPHY_close(gEthPhyHandle[instCnt]);
            gEthPhyHandle[instCnt] = NULL;
        }
    }
    return;
}

