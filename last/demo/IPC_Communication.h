#include <iostream>
#include "Communication.h"

using namespace std;


uint16_t gRemoteServiceEndPt = 13u;

RPMessage_Object gAckReplyMsgObject;


class IPC_Communication : public Communication {
    public:
        void open(uint16_t local_endPt) override {
            int32_t status;
            
            RPMessage_CreateParams createParams;

            RPMessage_CreateParams_init(&createParams);
            createParams.localEndPt = local_endPt;
            status = RPMessage_construct(&gAckReplyMsgObject, &createParams); 
            DebugP_assert(status == SystemP_SUCCESS);

            DebugP_log("Open IPC_Communication channel");
        }

        void close() override {
            DebugP_log("Closed IPC_Communication channel");
        }

        void write() override {
            DebugP_log("Writed something on the channel");
        }

        void read() override {
            DebugP_log("read something from the channel");        }

        void ioctl() override {
            DebugP_log("ioctl");
        }
};
