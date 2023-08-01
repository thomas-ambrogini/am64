#ifdef SOC_AM64X

#ifndef RPMESSAGE_COMMUNICATION_H
#define RPMESSAGE_COMMUNICATION_H

#include "Communication.hpp"
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include <drivers/ipc_rpmsg.h>
#include <drivers/ipc_notify.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <inttypes.h>
#include "RPMessageEndpoint.hpp"

class RPMessageCommunication : public Communication
{
public:
    RPMessageCommunication(const RPMessageEndpoint &Endpoint, const Logger &logger);

    ~RPMessageCommunication();

    int read(char *buffer, size_t bufferSize, Endpoint &source) override;
    int write(const char *message, size_t messageSize, const Endpoint &destination) override;

private:
    RPMessage_Object msgObject;
    const RPMessageEndpoint endpoint;
};

#endif
#endif
