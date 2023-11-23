#include "CommunicationUtils.hpp"

namespace CommunicationUtils
{
    void request(Communication *communication, CommunicationType communicationType, Endpoint *destinationEndpoint, const char *request, size_t requestSize, char *response, size_t responseSize)
    {
        communication->write(request, requestSize + 1, *destinationEndpoint);

        Endpoint *senderEndpoint = EndpointFactory::createEndpoint(communicationType);
        communication->read(response, responseSize, *senderEndpoint);

        delete senderEndpoint;
    }

}