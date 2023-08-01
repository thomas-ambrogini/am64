#ifndef BROKERMETADATA_H
#define BROKERMETADATA_H

#include <memory>
#include <vector>
#include "TopicMetadata.hpp"
#include "json.hpp"
#include "Endpoint.hpp"
#include "UDPEndpoint.hpp"
#include "RPMessageEndpoint.hpp"

using json = nlohmann::json;

class BrokerMetadata
{
public:
    BrokerMetadata();
    BrokerMetadata(Endpoint *endpoint);

    std::vector<TopicMetadata> getTopicsMetadata() const;
    void addTopicMetadata(const TopicMetadata t);
    Endpoint *getEndpoint() const;

    void setEndpoint(Endpoint *endpoint);

    void to_json(json &j) const;
    void from_json(const json &j);

private:
    Endpoint *endpoint;
    std::vector<TopicMetadata> topicsMetadata;
};

void to_json(json &j, const BrokerMetadata &obj);

#endif
