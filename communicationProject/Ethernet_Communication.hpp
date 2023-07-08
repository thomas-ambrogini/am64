#ifndef ETHERNET_COMMUNICATION_H
#define ETHERNET_COMMUNICATION_H

#include "Communication.hpp"

#define MAX_MSG_SIZE          (64u)

typedef struct EnetApp_PerCtxt_s
{
    /* Peripheral type */
    Enet_Type enetType;

    /* Peripheral instance */
    uint32_t instId;

    /* Peripheral's MAC ports to use */
    Enet_MacPort macPort;

    /* Name of this port to be used for logging */
    char *name;

    /* Enet driver handle for this peripheral type/instance */
    Enet_Handle hEnet;

    /* MAC address. It's port's MAC address in Dual-MAC or
     * host port's MAC addres in Switch */
    uint8_t macAddr[ENET_MAC_ADDR_LEN];

    /* TX channel number */
    uint32_t txChNum;

    /* TX channel handle */
    EnetDma_TxChHandle hTxCh;

    /* Start flow index */
    uint32_t rxStartFlowIdx;

    /* Regular traffic RX flow index */
    uint32_t rxFlowIdx;

    /* RX channel handle for regular traffic */
    EnetDma_RxChHandle hRxCh;

    /* RX task handle - receives Regular packets, changes source/dest MAC addresses
     * and transmits the packets back */
    TaskP_Object rxTaskObj;

    /* Semaphore posted from RX callback when Regular packets have arrived */
    SemaphoreP_Object rxSemObj;

    /* Semaphore used to synchronize all REgular RX tasks exits */
    SemaphoreP_Object rxDoneSemObj;

    /* Core key returned by Enet RM after attaching this core */
    uint32_t coreKey;

    /* Main UDMA driver handle */
    Udma_DrvHandle hMainUdmaDrv;

} EnetApp_PerCtxt;


typedef struct EnetApp_Obj_s
{
    /* Flag which indicates if test shall run */
    volatile bool run;

    /* This core's id */
    uint32_t coreId;

    /* Queue of free TX packets */
    EnetDma_PktQ txFreePktInfoQ;

    /* Array of all peripheral/port contexts used in the test */
    EnetApp_PerCtxt perCtxt[ENETAPP_PER_MAX];

    /* Number of active contexts being used */
    uint32_t numPerCtxts;
} EnetApp_Obj;

EnetApp_Obj gEnetApp;

class Ethernet_Communication : public Communication {
    private:
        RPMessage_Object MsgObject;
        uint32_t         gRemoteCoreId;
        uint16_t         gRemoteServiceEndPt;

        char ** util_destination(char * str);

    public:
        Ethernet_Communication(char * uri);

        ~Ethernet_Communication() {
            DebugP_log("Destroyed");
        }

        void create_endpoint(uint16_t localEndPt);

        //togliere la close e mettere il distruttore nel system
        void close();

        void write(char * msg) override;

        char* read() override;

        void ioctl() override;

};

#endif