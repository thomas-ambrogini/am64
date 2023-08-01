#ifdef SOC_AM64X

#ifndef TI_DEBUG_LOGGER_H
#define TI_DEBUG_LOGGER_H

#include "Logger.hpp"
#include <cstdio>
#include <cstring>
#include <kernel/dpl/DebugP.h>

class TI_DebugLogger : public Logger
{
public:
    void log(const char *format, ...) const override;
    void logError(const char *format, ...) const override;
};

#endif
#endif
