#ifndef ENDPOINTFACTORY_H
#define ENDPOINTFACTORY_H

#include "CommunicationType.hpp"
#include "UDPEndpoint.hpp"
#include "RPMessageEndpoint.hpp"

#ifdef SOC_AM64X
#include <inttypes.h>
#include <drivers/ipc_rpmsg.h>
#include <drivers/ipc_notify.h>
#endif

class EndpointFactory
{
public:
    static Endpoint *createEndpoint(CommunicationType commType);

private:
    static int freeEndpoint;

#ifdef SOC_AM64X
    static int getFreeEndpoint();
#endif
};

#endif
