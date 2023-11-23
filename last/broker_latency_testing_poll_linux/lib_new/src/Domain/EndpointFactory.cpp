#include "EndpointFactory.hpp"

int EndpointFactory::freeEndpoint = 1;

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
    {
#ifdef SOC_AM64X
        int endpoint = getFreeEndpoint();
        return new RPMessageEndpoint(IpcNotify_getSelfCoreId(), endpoint);
#endif
        break;
    }

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

#ifdef SOC_AM64X

static RPMessage_Object msgObject;

int EndpointFactory::getFreeEndpoint()
{
    int32_t status;
    for (int i = freeEndpoint; i < RPMESSAGE_MAX_LOCAL_ENDPT; i++)
    {
        RPMessage_CreateParams createParams;
        RPMessage_CreateParams_init(&createParams);
        createParams.localEndPt = i;
        status = RPMessage_construct(&msgObject, &createParams);

        if (status == 0)
        {
            RPMessage_destruct(&msgObject);
            freeEndpoint = i;
            return freeEndpoint;
        }
    }

    return -1;
}

#endif
