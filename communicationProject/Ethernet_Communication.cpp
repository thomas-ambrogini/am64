#include "Ethernet_Communication.hpp"

Ethernet_Communication::Ethernet_Communication(char * destination) {
    memset(&gEnetApp, 0, sizeof(gEnetApp));
    gEnetApp.run = true;

    gEnetApp.numPerCtxts = ENET_ARRAYSIZE(testParams);
}


char** Ethernet_Communication::util_destination(char* destination) {
    char ** tokens;

    tokens = str_split(destination, ';');
    
    return tokens;
}


void Ethernet_Communication::write(char * message) {

}

char * Ethernet_Communication::read() {

}

void Ethernet_Communication::ioctl() {
    DebugP_log("ioctl");
}