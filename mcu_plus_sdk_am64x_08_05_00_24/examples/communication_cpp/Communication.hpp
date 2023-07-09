#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/dpl/DebugP.h>
#include <inttypes.h>
#include <kernel/dpl/ClockP.h>
#include <drivers/ipc_rpmsg.h>
#include <drivers/ipc_notify.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include "utils_communication.hpp"

//avere info di cosa sone, per fare distruzioni/chiusure. Passare tipo al system per distruttore.
class Communication {
    public:
        //virtual functions
		virtual char* read()  = 0;
		virtual void write(char *msg) = 0;
		virtual void ioctl() = 0;

		virtual ~Communication() {}
};



#endif