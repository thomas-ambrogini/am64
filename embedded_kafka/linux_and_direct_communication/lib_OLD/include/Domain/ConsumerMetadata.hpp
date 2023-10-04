#ifndef CONSUMERMETADATA_H
#define CONSUMERMETADATA_H

#include "UDPEndpoint.hpp"
#include "json.hpp"

using json = nlohmann::json;

class ConsumerMetadata
{
public:
    ConsumerMetadata();
    ConsumerMetadata(int id, Endpoint *endpoint);

    int getId() const;

    void setEndpoint(Endpoint *endpoint);
    Endpoint *getEndpoint() const;

    bool operator<(const ConsumerMetadata &other) const;
    bool operator==(const ConsumerMetadata &other) const;

    void to_json(json &j) const;
    void from_json(const json &j);

private:
    int id;
    Endpoint *endpoint;
};

void to_json(nlohmann::json &j, const ConsumerMetadata &consumerMetadata);

#endif
