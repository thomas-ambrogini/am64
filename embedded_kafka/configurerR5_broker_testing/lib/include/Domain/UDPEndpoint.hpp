#ifndef UDPENDPOINT_H
#define UDPENDPOINT_H

#include "Endpoint.hpp"
#include <cstring>
#include "json.hpp"

using json = nlohmann::json;

class UDPEndpoint : public Endpoint
{
public:
    UDPEndpoint();
    UDPEndpoint(unsigned int p);
    UDPEndpoint(const char *ip, unsigned int p);

    ~UDPEndpoint() override;

    void printEndpointInformation(const Logger &logger) const override;

    void to_json(json &j) const override;
    void from_json(const json &j) override;

    void setIpAddress(const char *ip);
    void setPort(unsigned int port);
    const char *getIpAddress() const;
    unsigned int getPort() const;

private:
    char ipAddress[16];
    unsigned int port;
};

#endif
