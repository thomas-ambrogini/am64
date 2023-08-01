#include "EndpointFactory.hpp"

Endpoint *EndpointFactory::createEndpoint(CommunicationType commType)
{
    switch (commType)
    {
    case UDP:
#ifdef __unix__
        return new UDPEndpoint();
#endif
        break;

    case RPMessage:
#ifdef SOC_AM64X
        return new RPMessageEndpoint();
#endif
        break;

    case RPMessageLinux:
#ifdef __unix__
        return new RPMessageEndpoint();
#endif
        break;

    default:
        return nullptr;
    }

    return nullptr;
}
