#include <stdio.h>
#include <string.h>
#include <kernel/dpl/DebugP.h>
#include <inttypes.h>
#include <kernel/dpl/ClockP.h>
#include <drivers/ipc_rpmsg.h>
#include <drivers/ipc_notify.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

#define MAX_MSG_SIZE          (64u)


void open();
void close();
void read();
void write(uint32_t gRemoteCoreId, uint16_t gRemoteServiceEndPt);
void ioctl();