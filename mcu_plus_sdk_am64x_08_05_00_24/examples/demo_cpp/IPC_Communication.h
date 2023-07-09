#include "Communication.h"

#define MAX_MSG_SIZE          (64u)

RPMessage_Object gAckReplyMsgObject;

class IPC_Communication : public Communication {
    public:
        void open() override {
            int32_t status;
            DebugP_log("Open IPC_Communication channel\r\n");
            RPMessage_CreateParams createParams;

            RPMessage_CreateParams_init(&createParams);
            createParams.localEndPt = 12U;
            status = RPMessage_construct(&gAckReplyMsgObject, &createParams); 
            DebugP_assert(status == SystemP_SUCCESS);
	    }

        void close() override {
            RPMessage_destruct(&gAckReplyMsgObject);
            DebugP_log("Closed IPC_Communication channel\r\n");
        }

        void write(uint32_t gRemoteCoreId, uint16_t gRemoteServiceEndPt) override {
            char msgBuf[MAX_MSG_SIZE];
            uint32_t msg = 1, status;
            uint16_t msgSize;

            snprintf(msgBuf, MAX_MSG_SIZE - 1, "%d", msg);
            msgSize = strlen(msgBuf) + 1;

            status = RPMessage_send(
                msgBuf, msgSize,
                gRemoteCoreId,
                gRemoteServiceEndPt,
                RPMessage_getLocalEndPt(&gAckReplyMsgObject),
                SystemP_WAIT_FOREVER
            );

            DebugP_assert(status==SystemP_SUCCESS);

            DebugP_log("Wrote the message\r\n");


        }

        void read() override {
            char msgBuf[MAX_MSG_SIZE];
            uint32_t status;
            uint16_t msgSize, remoteCoreId, remoteCoreEndPt;
            DebugP_log("Ready to read the message\r\n");

            msgSize = sizeof(msgBuf);
            status = RPMessage_recv(&gAckReplyMsgObject,
                msgBuf, &msgSize,
                &remoteCoreId,
                &remoteCoreEndPt,
                SystemP_WAIT_FOREVER
            );
            DebugP_assert(status == SystemP_SUCCESS);
            
            DebugP_log("read something from the channel: %s", msgBuf);   

        }

        void ioctl() override {
            DebugP_log("ioctl");
        }
};
