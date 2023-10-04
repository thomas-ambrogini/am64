#ifdef __unix__

#include "StandardOutputLogger.hpp"

void StandardOutputLogger::log(const char *format, ...) const
{
    va_list args;
    va_start(args, format);

    vprintf(format, args);
    putchar('\n');

    va_end(args);

    std::cout.flush();
}

void StandardOutputLogger::logError(const char *format, ...) const
{
    va_list args;
    va_start(args, format);

    std::vfprintf(stderr, format, args);

    va_end(args);

    std::cout.flush();
}

#endif
