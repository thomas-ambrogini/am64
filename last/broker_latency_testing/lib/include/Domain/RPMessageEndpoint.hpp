#ifndef RPMESSAGEENDPOINT_H
#define RPMESSAGEENDPOINT_H

#include "Endpoint.hpp"
#include "Processor.hpp"

using nlohmann::json;

class RPMessageEndpoint : public Endpoint
{
public:
    RPMessageEndpoint();
    RPMessageEndpoint(uint16_t endpoint);
    RPMessageEndpoint(uint32_t coreId, uint16_t endpoint);

    ~RPMessageEndpoint() override;

    void printEndpointInformation(const Logger &logger) const override;

    void to_json(json &j) const override;
    void from_json(const json &j) override;

    void setCoreId(uint32_t coreId);
    void setServiceEndpoint(uint16_t serviceEndpoint);
    uint32_t getCoreId() const;
    uint16_t getServiceEndpoint() const;

    bool operator<(const RPMessageEndpoint &other) const;

private:
    uint32_t coreId;
    uint16_t serviceEndpoint;
};

#endif
