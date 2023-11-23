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
#include <kernel/dpl/TaskP.h>
#include <drivers/ipc_notify.h>
#include <drivers/ipc_rpmsg.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

#include "CommunicationFactory.hpp"
#include "EndpointFactory.hpp"
#include "RPMessageEndpoint.hpp"
#include "TI_DebugLogger.hpp"
#include "BrokerMetadata.hpp"
#include "json.hpp"

using json = nlohmann::json;

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

/* number of iterations of message exchange to do */
uint32_t gMsgEchoCount = 100000u;

/* maximum size that message can have */
#define MAX_MSG_SIZE        (1024u)

void fillBuffer(char * buffer, int bufferSize) {
    memset(buffer, 'c', bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
}

void sender(int localCoreId, int localServiceEndpoint, int remoteCoreId, int remoteServiceEndpoint) {
    uint32_t msg;    
    uint64_t curTime;
    uint16_t msgSize;
    char msgBuf[MAX_MSG_SIZE];
    TI_DebugLogger logger;

    Endpoint * sourceEndpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
    static_cast<RPMessageEndpoint *>(sourceEndpoint)->setCoreId(localCoreId);
    static_cast<RPMessageEndpoint *> (sourceEndpoint)->setServiceEndpoint(localServiceEndpoint);
    Communication * communication = CommunicationFactory::createCommunication(CommunicationType::RPMessage, *sourceEndpoint, logger );

    BrokerMetadata brokerMetadata(new RPMessageEndpoint(remoteCoreId, remoteServiceEndpoint));
       
    fillBuffer(msgBuf, 1024); 
    msgSize = strlen(msgBuf) + 1;
    
    logger.log("Message size: %d", msgSize);

    curTime = ClockP_getTimeUsec();

    Endpoint * endpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);

    for(msg=0; msg<gMsgEchoCount; msg++)
    {
        communication->write(msgBuf, msgSize, *brokerMetadata.getEndpoint());
        msgSize = sizeof(msgBuf);
        communication->read(msgBuf, msgSize, *endpoint);
    }

    curTime = ClockP_getTimeUsec() - curTime;

    DebugP_log("[IPC RPMSG ECHO] Number of messages sent = %d \r\n", gMsgEchoCount);
    DebugP_log("[IPC RPMSG ECHO] Total execution time = %" PRId64 " usecs\r\n", curTime);
    DebugP_log("[IPC RPMSG ECHO] One way message latency = %" PRId32 " nsec\r\n",
        (uint32_t)(curTime*1000u/(gMsgEchoCount)));


    delete endpoint;
    delete sourceEndpoint;
    delete communication;
}

void receiver(int localCoreId, int localServiceEndpoint) {
    char recvMsg[MAX_MSG_SIZE];
    uint16_t recvMsgSize;
    TI_DebugLogger logger;

    logger.log("Receiver started");

    Endpoint * sourceEndpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
    static_cast<RPMessageEndpoint *>(sourceEndpoint)->setCoreId(localCoreId);
    static_cast<RPMessageEndpoint *> (sourceEndpoint)->setServiceEndpoint(localServiceEndpoint);
    Communication * communication = CommunicationFactory::createCommunication(CommunicationType::RPMessage, *sourceEndpoint, logger );

    Endpoint * endpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
    
    while(1)
    {
        recvMsgSize = sizeof(recvMsg);
        communication->read(recvMsg, recvMsgSize, *endpoint);
        communication->write(recvMsg, recvMsgSize, *endpoint);
    }
}

void communication_main(void *args)
{
    int endpoint = 12;

    /* Open drivers to open the UART driver for console */
    Drivers_open();
    Board_driversOpen();

    DebugP_log("Started executing, i AM %d \r\n", IpcNotify_getSelfCoreId());

    if(IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS0_0) {
        IpcNotify_sendSync(CSL_CORE_ID_M4FSS0_0);
        sender(CSL_CORE_ID_R5FSS0_0, endpoint, CSL_CORE_ID_M4FSS0_0, endpoint);
    }
    else {
        IpcNotify_waitSync(CSL_CORE_ID_R5FSS0_0, SystemP_WAIT_FOREVER);
        receiver(IpcNotify_getSelfCoreId(), endpoint);
    }

    Board_driversClose();
    //Drivers_close();
}
