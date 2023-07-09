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

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/DebugP.h>
#include <drivers/ipc_notify.h>
#include <drivers/ipc_rpmsg.h>
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include "IPC_RPMessage.h"


#if defined(SOC_AM243X)
/* main core that starts the message exchange */
uint32_t gMainCoreId = CSL_CORE_ID_R5FSS0_0;
/* remote cores that echo messages from main core, make sure to NOT list main core in this list */
uint32_t gRemoteCoreId[] = {
    CSL_CORE_ID_M4FSS0_0,
    CSL_CORE_ID_R5FSS0_1,
    CSL_CORE_ID_R5FSS1_0,
    CSL_CORE_ID_R5FSS1_1,
    CSL_CORE_ID_MAX /* this value indicates the end of the array */
};
#endif

#if defined (SOC_AM263X)
/* main core that starts the message exchange */
uint32_t gMainCoreId = CSL_CORE_ID_R5FSS0_0;
/* remote cores that echo messages from main core, make sure to NOT list main core in this list */
uint32_t gRemoteCoreId[] = {
    CSL_CORE_ID_R5FSS0_1,
    CSL_CORE_ID_R5FSS1_0,
    CSL_CORE_ID_R5FSS1_1,
    CSL_CORE_ID_MAX /* this value indicates the end of the array */
};
#endif

#if defined(SOC_AM64X)
/* main core that starts the message exchange */
uint32_t gMainCoreId = CSL_CORE_ID_R5FSS0_0;
/* remote cores that echo messages from main core, make sure to NOT list main core in this list */
uint32_t gRemoteCoreId[] = {
    CSL_CORE_ID_M4FSS0_0,
    CSL_CORE_ID_R5FSS0_1,
    CSL_CORE_ID_R5FSS1_0,
    CSL_CORE_ID_R5FSS1_1,
    CSL_CORE_ID_A53SS0_0,
    CSL_CORE_ID_MAX /* this value indicates the end of the array */
};
#endif

#if defined(SOC_AM273X) || defined(SOC_AWR294X)
/* main core that starts the message exchange */
uint32_t gMainCoreId = CSL_CORE_ID_R5FSS0_0;
/* remote cores that echo messages from main core, make sure to NOT list main core in this list */
uint32_t gRemoteCoreId[] = {
    CSL_CORE_ID_R5FSS0_1,
    CSL_CORE_ID_C66SS0,
    CSL_CORE_ID_MAX /* this value indicates the end of the array */
};
#endif

/*
 * Remote core service end point
 *
 * pick any unique value on that core between 0..RPMESSAGE_MAX_LOCAL_ENDPT-1
 * the value need not be unique across cores
 */
uint16_t gRemoteServiceEndPt = 13u;

/* maximum size that message can have in this example */
#define MAX_MSG_SIZE        (64u)

/* Main core ack reply end point
 *
 * pick any unique value on that core between 0..RPMESSAGE_MAX_LOCAL_ENDPT-1
 * the value need not be unique across cores
 */
#define MAIN_CORE_ACK_REPLY_END_PT  (12U)


RPMessage_Object gAckReplyMsgObject;

void r5_0_0_code(void) {
    RPMessage_CreateParams createParams;
    uint32_t msg;
    char msgBuf[MAX_MSG_SIZE];
    int32_t status;
    uint16_t remoteCoreId, remoteCoreEndPt, msgSize;    

    RPMessage_CreateParams_init(&createParams);
    createParams.localEndPt = MAIN_CORE_ACK_REPLY_END_PT;
    status = RPMessage_construct(&gAckReplyMsgObject, &createParams);
    DebugP_assert(status==SystemP_SUCCESS);

    DebugP_log("Sending the sync \r\n");
    IpcNotify_sendSync(CSL_CORE_ID_R5FSS0_1);
    ClockP_usleep(500*1000);

    DebugP_log("Synced \r\n");


    msg=1;
    snprintf(msgBuf, MAX_MSG_SIZE-1, "%d", msg);
    msgBuf[MAX_MSG_SIZE-1] = 0;
    msgSize = strlen(msgBuf) + 1; /* count the terminating char as well */

    DebugP_log("Sending a message\r\n");


    status = RPMessage_send(
                msgBuf, msgSize,
                CSL_CORE_ID_R5FSS0_1, gRemoteServiceEndPt,
                RPMessage_getLocalEndPt(&gAckReplyMsgObject),
                SystemP_WAIT_FOREVER);
    DebugP_assert(status==SystemP_SUCCESS);

    DebugP_log("Message sent and waiting for ack\r\n");


    msgSize = sizeof(msgBuf);
    status = RPMessage_recv(&gAckReplyMsgObject,
        msgBuf, &msgSize,
        &remoteCoreId, &remoteCoreEndPt,
        SystemP_WAIT_FOREVER);
    DebugP_assert(status==SystemP_SUCCESS);

    DebugP_log("Received the ack\r\n");


    RPMessage_destruct(&gAckReplyMsgObject);

    DebugP_log("All tests have passed!!\r\n");
}

static RPMessage_Object gRecvMsgObject;

void r5_0_1_code(void) {
    int32_t status;
    RPMessage_CreateParams createParams;
    char recvMsg[MAX_MSG_SIZE];
    uint16_t recvMsgSize, remoteCoreId, remoteCoreEndPt;

    DebugP_log("Waiting the sync \r\n");
    IpcNotify_waitSync(CSL_CORE_ID_R5FSS0_0, SystemP_WAIT_FOREVER);

    DebugP_log("Exited the syncing \r\n");

    RPMessage_CreateParams_init(&createParams);
    createParams.localEndPt = gRemoteServiceEndPt;
    status = RPMessage_construct(&gRecvMsgObject, &createParams);
    DebugP_assert(status==SystemP_SUCCESS);

    while(1)
    {
        /* set 'recvMsgSize' to size of recv buffer,
        * after return `recvMsgSize` contains actual size of valid data in recv buffer
        */
        DebugP_log("Waiting to read a message \r\n");

        recvMsgSize = sizeof(recvMsg);
        status = RPMessage_recv(&gRecvMsgObject,
            recvMsg, &recvMsgSize,
            &remoteCoreId, &remoteCoreEndPt,
            SystemP_WAIT_FOREVER);
        DebugP_assert(status==SystemP_SUCCESS);
        DebugP_log("Received a message \r\n");

        /* echo the same message string as reply */

        DebugP_log("Sending the ack \r\n");

        /* send ack to sender CPU at the sender end point */
        status = RPMessage_send(
            recvMsg, recvMsgSize,
            remoteCoreId, remoteCoreEndPt,
            RPMessage_getLocalEndPt(&gRecvMsgObject),
            SystemP_WAIT_FOREVER);
        DebugP_assert(status==SystemP_SUCCESS);

        DebugP_log("Done \r\n");

    }
    /* This loop will never exit */
}

/*
 * This is an empty project provided for all cores present in the device.
 * User can use this project to start their application by adding more SysConfig modules.
 *
 * This application does driver and board init and just prints the pass string on the console.
 * In case of the main core, the print is redirected to the UART console.
 * For all other cores, CCS prints are used.
 */

void empty_main(void *args)
{
    /* Open drivers to open the UART driver for console */
    Drivers_open();
    Board_driversOpen();

    DebugP_log("I'm in the main before selecting which code to run and I am %d\r\n", IpcNotify_getSelfCoreId());

    if(IpcNotify_getSelfCoreId()==gMainCoreId)
    {
        r5_0_0_code();
    }
    else
    {
        r5_0_1_code();
    }

    DebugP_log("All tests have passed!!\r\n");

    Board_driversClose();
    Drivers_close();
}
