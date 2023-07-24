#include "BrokerMetadata.hpp"

BrokerMetadata::BrokerMetadata()
{
}

BrokerMetadata::BrokerMetadata(Endpoint *ep) : endpoint(ep)
{
}

void BrokerMetadata::to_json(json &j) const
{
    j = json{{"topics", topicsMetadata}};

    json polymorphicJson;
    endpoint->to_json(polymorphicJson);
    j["endpoint"] = polymorphicJson;
}

void BrokerMetadata::from_json(const json &j)
{
    json polymorphicJson = j.at("endpoint");
    std::string type = polymorphicJson.at("type").get<std::string>();

    if (type == "UDPSocket")
    {
        UDPEndpoint *udpEndpoint = new UDPEndpoint();
        udpEndpoint->from_json(polymorphicJson);
        endpoint = udpEndpoint;
    }

    for (const auto &item : j.at("topics"))
    {
        TopicMetadata obj;
        obj.from_json(item);
        topicsMetadata.push_back(obj);
    }
}

void to_json(json &j, const BrokerMetadata &obj)
{
    obj.to_json(j);
}

std::vector<TopicMetadata> BrokerMetadata::getTopicsMetadata() const
{
    return topicsMetadata;
}

void BrokerMetadata::addTopicMetadata(const TopicMetadata t)
{
    topicsMetadata.push_back(t);
}

Endpoint *BrokerMetadata::getEndpoint() const
{
    return endpoint;
}
