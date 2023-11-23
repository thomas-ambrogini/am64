#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <cstddef>
#include "Endpoint.hpp"
#include "Logger.hpp"

class Communication
{
public:
	Communication(const Logger &l) : logger(l) {}

	virtual ~Communication() {}

	virtual int write(const char *msg, const size_t size, const Endpoint &destination) = 0;
	virtual int read(char *buffer, const size_t size, Endpoint &source) = 0;

protected:
	const Logger &logger;
};

#endif
