#ifndef COMMUNICATION_UTILS_H
#define COMMUNICATION_UTILS_H

#include "Communication.hpp"
#include "EndpointFactory.hpp"

namespace CommunicationUtils
{
    void request(Communication *communication, CommunicationType communicationType, Endpoint *destinationEndpoint,
                 const char *request, size_t requestSize, char *response, size_t responseSize);
}

#endif
