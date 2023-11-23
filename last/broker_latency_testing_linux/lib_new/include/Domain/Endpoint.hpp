#ifndef ENDPOINT_H
#define ENDPOINT_H

#include "Logger.hpp"
#include "json.hpp"

using nlohmann::json;

class Endpoint
{
public:
    virtual ~Endpoint() {}

    virtual void printEndpointInformation(const Logger &logger) const = 0;

    virtual void to_json(json &j) const = 0;
    virtual void from_json(const json &j) = 0;
};

#endif
