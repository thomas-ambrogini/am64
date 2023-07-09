#include "IPC_Communication.hpp"

IPC_Communication::IPC_Communication (char * destination) {
    DebugP_log("Created a communication channel\r\n");
    DebugP_log("Destination: %s\r\n", destination);
    
    char ** elements = util_destination(destination);
    int32_t coreId   = str_to_coreId(elements[0]);
    int16_t endpoint = str_to_int16_t(elements[1]);

    free(elements[0]);
    free(elements[1]);

    DebugP_log("RemoteCoreID: %d\r\nEndpoint: %d\r\n", coreId, endpoint);

    this->gRemoteCoreId       = coreId;
    this->gRemoteServiceEndPt = endpoint;

    free(elements);

    create_endpoint(12);

}

char** IPC_Communication::util_destination(char* destination) {
    char ** tokens;

    tokens = str_split(destination, ';');
    
    return tokens;
}

void IPC_Communication::create_endpoint(uint16_t localEndPt) {
    int32_t status;
    DebugP_log("Open IPC_Communication channel\r\n");
    RPMessage_CreateParams createParams;

    RPMessage_CreateParams_init(&createParams);
    createParams.localEndPt = localEndPt;
    status = RPMessage_construct(&this->MsgObject, &createParams); 
    DebugP_assert(status == SystemP_SUCCESS);
}

void IPC_Communication::close() {
    RPMessage_destruct(&MsgObject);
    DebugP_log("Closed IPC_Communication channel\r\n");
}

void IPC_Communication::write(char * msg) {
    uint32_t status;
    uint16_t msgSize;

    msgSize = strlen(msg) + 1;

    status = RPMessage_send(
        msg, msgSize,
        gRemoteCoreId,
        gRemoteServiceEndPt,
        RPMessage_getLocalEndPt(&MsgObject),
        SystemP_WAIT_FOREVER
    );

    DebugP_assert(status==SystemP_SUCCESS);

}

char* IPC_Communication::read() {
    char msgBuf[MAX_MSG_SIZE];
    uint32_t status;
    uint16_t msgSize, remoteCoreId, remoteCoreEndPt;
    DebugP_log("Ready to read the message\r\n");

    msgSize = sizeof(msgBuf);
    status = RPMessage_recv(&MsgObject,
        msgBuf, &msgSize,
        &remoteCoreId,
        &remoteCoreEndPt,
        SystemP_WAIT_FOREVER
    );
    DebugP_assert(status == SystemP_SUCCESS);
                
    return strdup(msgBuf);
}

void IPC_Communication::ioctl() {
    DebugP_log("ioctl");
}