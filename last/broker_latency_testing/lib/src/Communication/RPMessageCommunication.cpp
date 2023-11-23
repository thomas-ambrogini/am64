#ifdef SOC_AM64X

#include "RPMessageCommunication.hpp"

RPMessageCommunication::RPMessageCommunication(const RPMessageEndpoint &ep, const Logger &l) : Communication(l), endpoint(ep)
{
    endpoint.printEndpointInformation(logger);
    int32_t status;
    RPMessage_CreateParams createParams;

    RPMessage_CreateParams_init(&createParams);
    createParams.localEndPt = endpoint.getServiceEndpoint();
    status = RPMessage_construct(&msgObject, &createParams);

    DebugP_assert(status == SystemP_SUCCESS);
}

RPMessageCommunication::~RPMessageCommunication()
{
    RPMessage_destruct(&msgObject);
}

int RPMessageCommunication::read(char *buffer, size_t bufferSize, Endpoint &source)
{
    uint32_t status;
    uint16_t remoteCoreId, remoteCoreServiceEndpoint;
    uint16_t size = static_cast<uint16_t>(bufferSize);

    status = RPMessage_recv(&msgObject,
                            buffer, &size,
                            &remoteCoreId,
                            &remoteCoreServiceEndpoint,
                            SystemP_WAIT_FOREVER);

    buffer[size] = 0; /* add a NULL char at the end of message */
    DebugP_assert(status == SystemP_SUCCESS);

    static_cast<RPMessageEndpoint &>(source).setCoreId(remoteCoreId);
    static_cast<RPMessageEndpoint &>(source).setServiceEndpoint(remoteCoreServiceEndpoint);

    return size;
}

int RPMessageCommunication::write(const char *message, size_t messageSize, const Endpoint &destination)
{
    const RPMessageEndpoint &rpMessageDestination = static_cast<const RPMessageEndpoint &>(destination);

    uint32_t status;
    uint16_t msgSize;

    msgSize = static_cast<uint16_t>(messageSize);

    status = RPMessage_send(
        const_cast<char *>(message), msgSize,
        rpMessageDestination.getCoreId(),
        rpMessageDestination.getServiceEndpoint(),
        RPMessage_getLocalEndPt(&msgObject),
        SystemP_WAIT_FOREVER);

    DebugP_assert(status == SystemP_SUCCESS);

    return 1;
}

#endif
