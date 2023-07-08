#include "SystemManager.hpp"


Communication * CommunicationFactory::open(CommunicationType commType, char* destination)
{
    if (commType == RPMessage) {
        if(!initialized) {
            init_RPMessage_communication(12);
            initialized = true;
            DebugP_log("Initialization completed\r\n");
        }

        IPC_Communication * comm = new IPC_Communication(destination);
        comm->setMsgObject(&staticMsgObject);
        return comm;
    }
    else
        return nullptr;
}



void CommunicationFactory::init_RPMessage_communication(uint16_t localEndPt) {
    int32_t status;
    DebugP_log("Open IPC_Communication channel\r\n");
    RPMessage_CreateParams createParams;

    RPMessage_CreateParams_init(&createParams);
    createParams.localEndPt = localEndPt;
    status = RPMessage_construct(&staticMsgObject, &createParams); 
    DebugP_assert(status == SystemP_SUCCESS);   
}
