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
#define MAX_MSG_SIZE        (128u)

void fillBuffer(char * buffer, int bufferSize) 
{
    memset(buffer, 'c', bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
}

void broker(int coreId, int serviceEndpoint) 
{
    TI_DebugLogger logger;
    RPMessageEndpoint brokerEndpoint(coreId, serviceEndpoint);

    Broker broker(CommunicationType::RPMessage, brokerEndpoint, logger);
    broker.printHandledTopic();
    
    logger.log("Start broker");
    broker.start();

}

void producer(int localCoreId, int localServiceEndpoint, int remoteCoreId, int remoteServiceEndpoint) 
{
    TI_DebugLogger logger;
    uint32_t msg;
    uint64_t curTime;
    char msgBuf[MAX_MSG_SIZE];


//    ClockP_usleep(2500*1000); /* wait for log messages from remote cores to be flushed, otherwise this delay is not needed */


    // Endpoint * sourceEndpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
    // static_cast<RPMessageEndpoint *>(sourceEndpoint)->setCoreId(localCoreId);
    // static_cast<RPMessageEndpoint *>(sourceEndpoint)->setServiceEndpoint(localServiceEndpoint); 
    // Communication * communication = CommunicationFactory::createCommunication(CommunicationType::RPMessage, *sourceEndpoint, logger);
    
    // BrokerMetadata timerService(new RPMessageEndpoint(remoteCoreId, remoteServiceEndpoint));

    Producer producer(CommunicationType::RPMessage, logger);
    logger.log("Created the producer");

    Consumer consumer(CommunicationType::RPMessage, logger);
    logger.log("Created the consumer");

    TopicMetadata topicMetadata("Prova");

    consumer.subscribe(topicMetadata);

    ClockP_usleep(5000*1000);

    fillBuffer(msgBuf, 32-16);

    Record record("hellooooooooooo");
    ProducerRecord producerRecord(topicMetadata, record);

    curTime = ClockP_getTimeUsec();

    for(msg = 0; msg < gMsgEchoCount; msg++) {
        producer.publish(producerRecord);
        consumer.read(topicMetadata);
    }
    curTime = ClockP_getTimeUsec() - curTime;

    logger.log("Total Execution time = %" PRId64 " usecs", curTime);
    logger.log("Time for the exchange = %" PRId32 " nsec", (uint32_t)(curTime*1000u/(gMsgEchoCount *2)));

}

void consumer(int localCoreId, int localServiceEndpoint, int remoteCoreId, int remoteServiceEndpoint) 
{
    TI_DebugLogger logger;
    uint32_t msg;
    // uint16_t msgSize;
    uint64_t curTime;
    // char msgBuf[MAX_MSG_SIZE];

    // Endpoint * sourceEndpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
    // static_cast<RPMessageEndpoint *>(sourceEndpoint)->setCoreId(localCoreId);
    // static_cast<RPMessageEndpoint *>(sourceEndpoint)->setServiceEndpoint(localServiceEndpoint); 
    // Communication * communication = CommunicationFactory::createCommunication(CommunicationType::RPMessage, *sourceEndpoint, logger);
    
    // BrokerMetadata timerService(new RPMessageEndpoint(remoteCoreId, remoteServiceEndpoint));


    Consumer consumer(CommunicationType::RPMessage, logger);
    logger.log("Created the consumer");

    TopicMetadata topicMetadata("Prova");

    consumer.subscribe(topicMetadata);

    IpcNotify_sendSync(CSL_CORE_ID_R5FSS0_1);


    curTime = ClockP_getTimeUsec();

    for(msg = 0; msg < gMsgEchoCount; msg++) {
        consumer.read(topicMetadata);
        logger.log("Read %d messages", msg + 1);
    }

    // fillBuffer(msgBuf, 20-16);
    // msgSize = strlen(msgBuf) + 1;
    // communication->write(msgBuf, msgSize, *timerService.getEndpoint());

    curTime = ClockP_getTimeUsec() - curTime;

    logger.log("Total Execution time = %" PRId64 " usecs", curTime);
    logger.log("Time for the exchange = %" PRId32 " nsec", (uint32_t)(curTime*1000u/(gMsgEchoCount *2)));
}

void timer(int localCoreId, int localServiceEndpoint) {
    TI_DebugLogger logger;
    uint64_t curTime;
    char msgBuf[MAX_MSG_SIZE];

    Endpoint * sourceEndpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
    static_cast<RPMessageEndpoint *>(sourceEndpoint)->setCoreId(localCoreId);
    static_cast<RPMessageEndpoint *>(sourceEndpoint)->setServiceEndpoint(localServiceEndpoint); 
    Communication * communication = CommunicationFactory::createCommunication(CommunicationType::RPMessage, *sourceEndpoint, logger);

    Endpoint* endpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
    communication->read(msgBuf, sizeof(msgBuf), *endpoint);
    curTime = ClockP_getTimeUsec();

    communication->read(msgBuf, sizeof(msgBuf), *endpoint);
    curTime = ClockP_getTimeUsec() - curTime;

    logger.log("Total Execution time = %" PRId64 " usecs", curTime);
    logger.log("Time for the exchange = %" PRId32 " nsec", (uint32_t)(curTime*1000u/(gMsgEchoCount *2)));
}

void communication_main(void *args)
{
    int endpoint = 14;

    Drivers_open();
    Board_driversOpen();

    DebugP_log("Started executing, i AM %d \r\n", IpcNotify_getSelfCoreId());

    if(IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS0_0) {
        broker(CSL_CORE_ID_R5FSS0_0, 12);
    }
    else if(IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS0_1) {
        producer(CSL_CORE_ID_R5FSS0_1, endpoint, CSL_CORE_ID_R5FSS1_1, endpoint);
    }
    else if(IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS1_0) {
        consumer(CSL_CORE_ID_R5FSS1_0, endpoint,CSL_CORE_ID_R5FSS1_1, endpoint );
    }
    else if(IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS1_1) {
        timer(CSL_CORE_ID_R5FSS1_1, endpoint);
    }

    Board_driversClose();
    //Drivers_close();
}
