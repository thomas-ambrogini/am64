#ifndef ENDPOINTFACTORY_H
#define ENDPOINTFACTORY_H

#include "CommunicationType.hpp"
#include "UDPEndpoint.hpp"
#include "RPMessageEndpoint.hpp"

class EndpointFactory
{
public:
    static Endpoint *createEndpoint(CommunicationType commType);
};

#endif
