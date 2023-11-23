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
#include "Broker.hpp"
#include "Producer.hpp"
#include "Consumer.hpp"
#include "ProducerRecord.hpp"
#include "Configurer.hpp"

using json = nlohmann::json;

/* This is used to run the echo test with user space kernel */
#define IPC_RPMESSAGE_SERVICE_CHRDEV "rpmsg_chrdev"
#define IPC_RPMESSAGE_ENDPT_CHRDEV_PING (14U)

/* maximum size that message can have */
#define MAX_MSG_SIZE (1024u)

uint32_t gMsgEchoCount = 100000u;


void waitingLinux()
{
    int32_t status;

    /* This API MUST be called by applications when its ready to talk to Linux */
    status = RPMessage_waitForLinuxReady(SystemP_WAIT_FOREVER);
    DebugP_assert(status == SystemP_SUCCESS);

    status = RPMessage_announce(CSL_CORE_ID_A53SS0_0, IPC_RPMESSAGE_ENDPT_CHRDEV_PING, IPC_RPMESSAGE_SERVICE_CHRDEV);
    DebugP_assert(status == SystemP_SUCCESS);
}

void fillBuffer(char * buffer, int bufferSize) 
{
    memset(buffer, 'c', bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
}

void configurer(int coreId, int serviceEndpoint)
{
    TI_DebugLogger logger;

    RPMessageEndpoint *sourceEndpoint = static_cast<RPMessageEndpoint *>(EndpointFactory::createEndpoint(CommunicationType::RPMessage));
    sourceEndpoint->setCoreId(coreId);
    sourceEndpoint->setServiceEndpoint(serviceEndpoint);

    Configurer configurer(CommunicationType::RPMessage, *sourceEndpoint, logger);
    configurer.start();

    delete sourceEndpoint;
}

void broker(int coreId, int serviceEndpoint)
{
    TI_DebugLogger logger;
    RPMessageEndpoint brokerEndpoint(coreId, serviceEndpoint);
    BrokerMetadata configurerMetadata(new RPMessageEndpoint(CSL_CORE_ID_R5FSS0_0, 14));

    Broker broker(CommunicationType::RPMessage, brokerEndpoint, logger, configurerMetadata);

    logger.log("Start broker");
    broker.start();
}

void receiver(int localCoreId, int localServiceEndpoint)
{
    char recvMsg[MAX_MSG_SIZE];
    uint16_t recvMsgSize;
    TI_DebugLogger logger;

    Endpoint *sourceEndpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
    static_cast<RPMessageEndpoint *>(sourceEndpoint)->setCoreId(localCoreId);
    static_cast<RPMessageEndpoint *>(sourceEndpoint)->setServiceEndpoint(localServiceEndpoint);
    Communication *communication = CommunicationFactory::createCommunication(CommunicationType::RPMessage, *sourceEndpoint, logger);

    Endpoint *endpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);

    while (1)
    {
        recvMsgSize = sizeof(recvMsg);
        communication->read(recvMsg, recvMsgSize, *endpoint);
        communication->write(recvMsg, recvMsgSize, *endpoint);
    }
}

void producer(int coreId, int serviceEndpoint)
{
    TI_DebugLogger logger;
    RPMessageEndpoint brokerEndpoint(coreId, serviceEndpoint);

    BrokerMetadata configurerMetadata(new RPMessageEndpoint(CSL_CORE_ID_R5FSS0_0, 14));

    Producer producer(CommunicationType::RPMessage, logger, configurerMetadata);

    // Broker broker(CommunicationType::RPMessage, brokerEndpoint, logger, configurerMetadata);
    // broker.printHandledTopic();
}

void consumer(int coreId, int serviceEndpoint)
{
    TI_DebugLogger logger;
    RPMessageEndpoint brokerEndpoint(coreId, serviceEndpoint);

    BrokerMetadata configurerMetadata(new RPMessageEndpoint(CSL_CORE_ID_A53SS0_0, 14));

    Consumer consumer(CommunicationType::RPMessage, logger, configurerMetadata);

    // Broker broker(CommunicationType::RPMessage, brokerEndpoint, logger, configurerMetadata);
    // broker.printHandledTopic();
}

void tester(int localCoreId, int localServiceEndpoint,std::string topicName) 
{
    TI_DebugLogger logger;
    uint32_t msg;
    char msgBuf[MAX_MSG_SIZE];
    uint64_t curTime;

    BrokerMetadata configurerMetadata(new RPMessageEndpoint(CSL_CORE_ID_R5FSS0_0, 14));


    Producer producer(CommunicationType::RPMessage, logger, configurerMetadata);
    logger.log("Created the producer");
    Consumer consumer(CommunicationType::RPMessage, logger, configurerMetadata);
    logger.log("Created the consumer");

    TopicMetadata topicMetadata(topicName);

    consumer.subscribe(topicMetadata);

    ClockP_usleep(5000*1000);

    fillBuffer(msgBuf, 8); 

    Record record(msgBuf);
    ProducerRecord producerRecord(topicMetadata, record);

    curTime = ClockP_getTimeUsec();

    for(msg = 0; msg < gMsgEchoCount; msg++) {
        producer.publish(producerRecord);
        logger.log("Published a message");
        consumer.read(topicMetadata);
    }

    curTime = ClockP_getTimeUsec() - curTime;

    logger.log("Total Execution time = %" PRId64 " usecs", curTime);
    logger.log("Time for the exchange = %" PRId32 " nsec", (uint32_t)(curTime*1000u/(gMsgEchoCount)));

}

void communication_main(void *args)
{
    int endpoint = 14;

    Drivers_open();
    Board_driversOpen();
    TI_DebugLogger logger;

    logger.log("Started, I am %d", IpcNotify_getSelfCoreId());

    // waitingLinux();

    if (IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS0_0)
    {
        configurer(IpcNotify_getSelfCoreId(), endpoint);
    }
    else if (IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS0_1)
    {
        broker(CSL_CORE_ID_R5FSS0_1, 13);
    }
    else if (IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS1_0)
    {
        tester(CSL_CORE_ID_R5FSS1_0, endpoint, "Testing");
    }

    Board_driversClose();
    // Drivers_close();
}
