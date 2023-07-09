#ifndef UTILS_COMMUNICATION_H
#define UTILS_COMMUNICATION_H

#include "assert.h"
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <iostream>
#include <drivers/ipc_notify.h>
#include <drivers/ipc_rpmsg.h>


char ** str_split(char * str, const char delimiter);

uint16_t str_to_int16_t     (char * str);

uint32_t str_to_coreId      (char * str);

#endif