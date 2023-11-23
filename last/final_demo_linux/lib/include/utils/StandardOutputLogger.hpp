#ifdef __unix__

#ifndef STANDARD_OUTPUT_LOGGER_H
#define STANDARD_OUTPUT_LOGGER_H

#include "Logger.hpp"
#include <iostream>

class StandardOutputLogger : public Logger
{
public:
    void log(const char *format, ...) const override;
    void logError(const char *format, ...) const override;
};

#endif
#endif
