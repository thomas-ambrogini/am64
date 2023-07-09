#ifndef SYSTEM_INFORMATION_H
#define SYSTEM_INFORMATION_H

#include "Communication.hpp"
#include "IPC_Communication.hpp"

enum CommunicationType{
    RPMessage,
    IPCNotify,
    Ethernet
};

enum Processors {
    M4,
    A53_0,
    A53_1,
    R5_0_0,
    R5_0_1,
    R5_1_0,
    R5_1_1
};

//void * per il destination, il puntatore ad una struttura
//reinterpret cast, non sai tipo finale, ma provi a custarlo a qualcosa che centra
class CommunicationFactory {
    public:
        static Communication * open(CommunicationType type, char * destination);
};

#endif