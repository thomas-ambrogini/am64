#include "CommunicationFactory.hpp"

Communication *CommunicationFactory::createCommunication(CommunicationType commType, const Endpoint &endpoint, const Logger &logger)
{
    switch (commType)
    {
    case UDP:
#ifdef __unix__
        return new UDPSocketCommunication(dynamic_cast<const UDPEndpoint &>(endpoint), logger);
#endif
        break;

    case RPMessage:
#ifdef SOC_AM64X
        return new RPMessageCommunication(static_cast<const RPMessageEndpoint &>(endpoint), logger);
#endif
        break;

    case RPMessageLinux:
#ifdef __unix__
        return new RPMessageLinuxCommunication(static_cast<const RPMessageEndpoint &>(endpoint), logger);
#endif
        break;

    default:
        return nullptr;
    }

    return nullptr;
}