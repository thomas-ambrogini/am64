#ifdef __unix__

#include "UDPSocketCommunication.hpp"

UDPSocketCommunication::UDPSocketCommunication(const UDPEndpoint &ep, const Logger &l) : Communication(l), endpoint(ep)
{
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(endpoint.getPort());
    if (inet_aton(endpoint.getIpAddress(), &serverAddress.sin_addr) == 0)
    {
        logger.logError("Invalid IP address, bynding to INADDR_ANY");
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    open();
}

UDPSocketCommunication::UDPSocketCommunication(const char *ipAddress, unsigned int port, const Logger &l) : Communication(l), endpoint(UDPEndpoint(ipAddress, port))
{
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_aton(ipAddress, &serverAddress.sin_addr) == 0)
    {
        logger.logError("Invalid IP address");
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    open();
}

void UDPSocketCommunication::open()
{
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd == -1)
    {
        logger.logError("Failed to create socket.");
    }

    if (bind(socketfd, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        logger.logError("Failed to bind socket.");
        ::close(socketfd);
    }
}

UDPSocketCommunication::~UDPSocketCommunication()
{
    if (socketfd != -1)
    {
        ::close(socketfd);
        socketfd = -1;
    }
}

int UDPSocketCommunication::read(char *buffer, size_t bufferSize, Endpoint &source)
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLength = sizeof(clientAddr);

    ssize_t receivedBytes = recvfrom(socketfd, buffer, bufferSize, 0, (struct sockaddr *)&clientAddr, &clientAddrLength);
    if (receivedBytes < 0)
    {
        logger.logError("Failed to receive data");
        return -1;
    }

    // Populate the source information
    char ipAddress[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(clientAddr.sin_addr), ipAddress, INET_ADDRSTRLEN) == nullptr)
    {
        logger.logError("Failed to get source IP address");
        return -1;
    }

    unsigned int port = ntohs(clientAddr.sin_port);

    try
    {
        dynamic_cast<UDPEndpoint &>(source).setIpAddress(ipAddress);
        dynamic_cast<UDPEndpoint &>(source).setPort(port);
    }
    catch (const std::bad_cast &e)
    {
        logger.logError("Bad cast, the source information will not be returned");
    }

    return receivedBytes;
}

int UDPSocketCommunication::write(const char *message, size_t messageSize, const Endpoint &destination)
{
    const UDPEndpoint &udpDestination = dynamic_cast<const UDPEndpoint &>(destination);

    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(udpDestination.getPort());
    if (inet_aton(udpDestination.getIpAddress(), &destAddr.sin_addr) == 0)
    {
        logger.logError("Invalid destination address");
        return -1;
    }

    ssize_t sentBytes = sendto(socketfd, message, messageSize, 0, (struct sockaddr *)&destAddr, sizeof(destAddr));
    if (sentBytes < 0)
    {
        logger.logError("Failed to write the message.");
        return -1;
    }

    return sentBytes;
}

#endif
