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
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include "SystemManager.hpp"

/*
 * This is an empty project provided for all cores present in the device.
 * User can use this project to start their application by adding more SysConfig modules.
 *
 * This application does driver and board init and just prints the pass string on the console.
 * In case of the main core, the print is redirected to the UART console.
 * For all other cores, CCS prints are used.
 */
void r5_0_0_code(void) {
    char uri[MAX_MSG_SIZE];
    snprintf(uri, MAX_MSG_SIZE-1, "%s", "R5_0_1;12");
    uri[MAX_MSG_SIZE-1] = 0;

    CommunicationFactory * cf = new CommunicationFactory();
    Communication * comm = cf->open(RPMessage, uri);
    
    char * msg = comm->read();
    
    DebugP_log("Message Received: %s \r\n", msg);

    delete comm;
    delete cf;
}

void r5_0_1_code(void) {
    char uri[MAX_MSG_SIZE];
    snprintf(uri, MAX_MSG_SIZE-1, "%s", "R5_0_0;12");
    uri[MAX_MSG_SIZE-1] = 0;

    ClockP_usleep(500*10000); /* wait for log messages from remote cores to be flushed, otherwise this delay is not needed */

    // IPC_Communication channel;
    // channel.write(msg);

    CommunicationFactory * cf = new CommunicationFactory();
    Communication * comm = cf->open(RPMessage, uri);
    comm->write("Hello R5!");
    
    delete comm;
    delete cf;
}


void m4_code() {
    char uri[MAX_MSG_SIZE];
    snprintf(uri, MAX_MSG_SIZE-1, "%s", "R5_0_0;12");
    uri[MAX_MSG_SIZE-1] = 0;

    ClockP_usleep(500*10000); /* wait for log messages from remote cores to be flushed, otherwise this delay is not needed */

    // IPC_Communication channel;
    // channel.write(msg);

    CommunicationFactory * cf = new CommunicationFactory();
    Communication * comm = cf->open(RPMessage, uri);
    DebugP_log("M4 Sending the message \r\n");


    comm->write("Hello R5 From M4!");
    
    delete comm;
    delete cf;


}

void communication_main(void *args)
{
    /* Open drivers to open the UART driver for console */
    Drivers_open();
    Board_driversOpen();

    DebugP_log("Started executing, i AM %d \r\n", IpcNotify_getSelfCoreId());

    if(IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS0_0) {
        r5_0_0_code();
    }
    else if (IpcNotify_getSelfCoreId() == CSL_CORE_ID_R5FSS0_1){
        r5_0_1_code();
    }
    else if (IpcNotify_getSelfCoreId() == CSL_CORE_ID_M4FSS0_0){
        m4_code();
    }

//    DebugP_log("Main started!!\r\n");

    Board_driversClose();
    Drivers_close();
}
