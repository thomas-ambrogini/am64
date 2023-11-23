#ifdef __unix__

#ifndef UDPSOCKET_COMMUNICATION_H
#define UDPSOCKET_COMMUNICATION_H

#include "Communication.hpp"
#include "Logger.hpp"
#include "UDPEndpoint.hpp"

#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <typeinfo>

class UDPSocketCommunication : public Communication
{
public:
    UDPSocketCommunication(const UDPEndpoint &Endpoint, const Logger &logger);
    UDPSocketCommunication(const char *ip, unsigned int port, const Logger &logger);

    ~UDPSocketCommunication() override;

    int read(char *buffer, size_t bufferSize, Endpoint &source) override;
    int write(const char *message, size_t messageSize, const Endpoint &destination) override;

private:
    int socketfd;
    struct sockaddr_in serverAddress;
    const UDPEndpoint endpoint;

    void open();
};

#endif

#endif
