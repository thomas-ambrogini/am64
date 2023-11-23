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

#include "TI_DebugLogger.hpp"
#include "BrokerMetadata.hpp"
#include "json.hpp"
#include "Producer.hpp"
#include "Consumer.hpp"
#include "ProducerRecord.hpp"

using json = nlohmann::json;


/* number of iterations of message exchange to do */
uint32_t gMsgEchoCount = 2000u;

/* maximum size that message can have */
#define MAX_MSG_SIZE        (1024u)

void fillBuffer(char * buffer, int bufferSize) 
{
    memset(buffer, 'c', bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
}

void producer(int localCoreId, int localServiceEndpoint, std::string topicName)
{
    TI_DebugLogger logger;
    logger.setDebug(false);
    uint32_t msg;
    char msgBuf[MAX_MSG_SIZE];
    uint64_t curTime;

    BrokerMetadata configurerMetadata(new RPMessageEndpoint(CSL_CORE_ID_R5FSS0_0, 14));

    Producer producer(CommunicationType::RPMessage, logger, configurerMetadata, true);

    TopicMetadata topicMetadata(topicName);

    fillBuffer(msgBuf, 32); 

    Record record(msgBuf);
    ProducerRecord producerRecord(topicMetadata, record);

    curTime = ClockP_getTimeUsec();

    for(msg = 0; msg < gMsgEchoCount; msg++) {
        producer.publish(producerRecord);
    }

    curTime = ClockP_getTimeUsec() - curTime;

    IpcNotify_sendSync(CSL_CORE_ID_R5FSS0_1);

    DebugP_log("Total Execution time for the producer = %" PRId64 " usecs \r\n", curTime);
    DebugP_log("Time for the exchange for the producer = %" PRId32 " nsec \r\n", (uint32_t)(curTime*1000u/(gMsgEchoCount)));
}

void consumer(int localCoreId, int localServiceEndpoint, std::string topicName)
{
    TI_DebugLogger logger;
    logger.setDebug(false);
    uint32_t msg;
    uint64_t curTime;
    int numberMessagePolled = 0;

    BrokerMetadata configurerMetadata(new RPMessageEndpoint(CSL_CORE_ID_R5FSS0_0, 14));

    Consumer consumer(CommunicationType::RPMessage, logger, configurerMetadata, true);
    logger.log("Created the consumer");

    TopicMetadata topicMetadata(topicName);

    consumer.subscribe(topicMetadata);
    Record record;

    IpcNotify_waitSync(CSL_CORE_ID_R5FSS0_1, SystemP_WAIT_FOREVER);

    curTime = ClockP_getTimeUsec();

    DebugP_log("Starting the messages polling \r\n");

    for(msg = 0; msg < gMsgEchoCount; msg++) {
        record = consumer.poll(topicMetadata);
            
        if(record.getData() != "NOT VALID") {
            numberMessagePolled++;
        }
    }

    curTime = ClockP_getTimeUsec() - curTime;

    DebugP_log("Total Execution time for the consumer = %" PRId64 " usecs \r\n", curTime);
    DebugP_log("Time for the exchange for the consumer = %" PRId32 " nsec \r\n", (uint32_t)(curTime*1000u/(gMsgEchoCount)));
    DebugP_log("Number message (correct) polled = %d\r\n", numberMessagePolled);
}

// void tester(int localCoreId, int localServiceEndpoint,std::string topicName) 
// {
//     TI_DebugLogger logger;
//     logger.setDebug(false);
//     uint32_t msg;
//     char msgBuf[MAX_MSG_SIZE];
//     uint64_t curTime;

//     BrokerMetadata configurerMetadata(new RPMessageEndpoint(CSL_CORE_ID_R5FSS0_0, 14));

//     Producer producer(CommunicationType::RPMessage, logger, configurerMetadata, true);
//     logger.log("Created the producer");
//     Consumer consumer(CommunicationType::RPMessage, logger, configurerMetadata, true);
//     logger.log("Created the consumer");

//     TopicMetadata topicMetadata(topicName);

//     consumer.subscribe(topicMetadata);

//     ClockP_usleep(5000*1000);

//     fillBuffer(msgBuf, 924); 

//     Record record(msgBuf);
//     ProducerRecord producerRecord(topicMetadata, record);

//     curTime = ClockP_getTimeUsec();

//     for(msg = 0; msg < gMsgEchoCount; msg++) {
//         producer.publish(producerRecord);
//         consumer.read(topicMetadata);
//     }

//     curTime = ClockP_getTimeUsec() - curTime;

//     DebugP_log("Total Execution time = %" PRId64 " usecs \r\n", curTime);
//     DebugP_log("Time for the exchange = %" PRId32 " nsec \r\n", (uint32_t)(curTime*1000u/(gMsgEchoCount)));

// }

void communication_main_m4(void *args)
{
    int endpoint = 14;

    Drivers_open();
    Board_driversOpen();

    DebugP_log("Started executing, i AM %d \r\n", IpcNotify_getSelfCoreId());

    consumer(CSL_CORE_ID_M4FSS0_0, endpoint, "Testing");


    Board_driversClose();
    //Drivers_close();
}
