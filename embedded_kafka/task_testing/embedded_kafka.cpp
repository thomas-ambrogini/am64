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
#include <cstdlib>
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

#define NUM_TASKS (2u)

/* Task priority, stack, stack size and task objects, these MUST be global's */
#define TASK_PRI (8U)
#define TASK_STACK_SIZE (8 * 1024U)
uint8_t gIpcTaskStack[NUM_TASKS][TASK_STACK_SIZE] __attribute__((aligned(32)));
TaskP_Object gIpcTask[NUM_TASKS];

void waitingLinux()
{
    int32_t status;

    /* This API MUST be called by applications when its ready to talk to Linux */
    status = RPMessage_waitForLinuxReady(SystemP_WAIT_FOREVER);
    DebugP_assert(status == SystemP_SUCCESS);

    status = RPMessage_announce(CSL_CORE_ID_A53SS0_0, IPC_RPMESSAGE_ENDPT_CHRDEV_PING, IPC_RPMESSAGE_SERVICE_CHRDEV);
    DebugP_assert(status == SystemP_SUCCESS);
}

void configurer(void *pvParameters)
{
    TI_DebugLogger logger;
    int coreId = CSL_CORE_ID_R5FSS0_0;
    int serviceEndpoint = 14;

    RPMessageEndpoint *sourceEndpoint = static_cast<RPMessageEndpoint *>(EndpointFactory::createEndpoint(CommunicationType::RPMessage));
    sourceEndpoint->setCoreId(coreId);
    sourceEndpoint->setServiceEndpoint(serviceEndpoint);

    Configurer configurer(CommunicationType::RPMessage, *sourceEndpoint, logger);
    configurer.start();

    delete sourceEndpoint;
}

void producer()
{
    TI_DebugLogger logger;

    BrokerMetadata configurerMetadata(new RPMessageEndpoint(CSL_CORE_ID_R5FSS0_0, 14));

    Producer producer(CommunicationType::RPMessage, logger, configurerMetadata);

    std::string topicName = "Testing";
    TopicMetadata topic(topicName);

    while (true)
    {
        float randomNumber = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * 90.0f;
        std::string randomString = std::to_string(randomNumber);

        ProducerRecord producerRecord(topicName, randomString);
        producer.publish(producerRecord);

        ClockP_usleep(10000 * 1000);
    }
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

void Task1(void *pvParameters)
{
    while (1)
    {
        DebugP_log("[Task 1] Hello !!!\r\n");
        ClockP_usleep(1000 * 1000);
    }
}

void Task2(void *pvParameters)
{
    while (1)
    {
        DebugP_log("[Task 2] Hello !!!\r\n");
        ClockP_usleep(10000 * 1000);
    }
}

void create_tasks(void)
{
    int32_t status;
    TaskP_Params taskParams;

    /* Create the tasks which will handle service */
    TaskP_Params_init(&taskParams);
    taskParams.name = "configurer";
    taskParams.stackSize = TASK_STACK_SIZE;
    taskParams.stack = gIpcTaskStack[0];
    taskParams.priority = TASK_PRI;
    /* we use the same task function for echo but pass the appropiate rpmsg handle to it, to echo messages */
    taskParams.taskMain = configurer;

    status = TaskP_construct(&gIpcTask[0], &taskParams);
    DebugP_assert(status == SystemP_SUCCESS);

    /* Create the tasks which will handle service */
    TaskP_Params_init(&taskParams);
    taskParams.name = "Task2";
    taskParams.stackSize = TASK_STACK_SIZE;
    taskParams.stack = gIpcTaskStack[1];
    taskParams.priority = TASK_PRI;
    /* we use the same task function for echo but pass the appropiate rpmsg handle to it, to echo messages */
    taskParams.taskMain = Task2;

    status = TaskP_construct(&gIpcTask[1], &taskParams);
    DebugP_assert(status == SystemP_SUCCESS);
}

void communication_main(void *args)
{
    // Init hardware and peripherals
    Drivers_open();
    Board_driversOpen();

    // TI_DebugLogger logger;

    // logger.log("Started, I am %d", IpcNotify_getSelfCoreId());
    DebugP_log("Started, I am %d \r\n", IpcNotify_getSelfCoreId());

    if (IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS0_0)
    {
        create_tasks();
        // configurer(IpcNotify_getSelfCoreId(), endpoint);
    }
    else if (IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS0_1)
    {
        broker(CSL_CORE_ID_R5FSS0_1, 13);
    }
    else if (IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS1_0)
    {
        producer();
    }

    Board_driversClose();
    // Drivers_close();
}