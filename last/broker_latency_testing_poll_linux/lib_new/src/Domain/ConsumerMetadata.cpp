#include "ConsumerMetadata.hpp"

ConsumerMetadata::ConsumerMetadata() : id(-1), endpoint(nullptr)
{
}

ConsumerMetadata::ConsumerMetadata(int value, Endpoint *ep) : id(value), endpoint(ep)
{
}

int ConsumerMetadata::getId() const
{
    return id;
}

Endpoint *ConsumerMetadata::getEndpoint() const
{
    return endpoint;
}

void ConsumerMetadata::setEndpoint(Endpoint *ep)
{
    endpoint = ep;
}

void ConsumerMetadata::to_json(json &j) const
{
    j = json{{"id", id}};

    if (endpoint != nullptr)
    {
        json polymorphicJson;
        endpoint->to_json(polymorphicJson);
        j["endpoint"] = polymorphicJson;
    }
}

void ConsumerMetadata::from_json(const json &j)
{
    id = j.at("id").get<int>();

    if (j.contains("enpoint"))
    {
        json polymorphicJson = j.at("endpoint");
        std::string type = polymorphicJson.at("type").get<std::string>();
        if (type == "UDPSocket")
        {
            UDPEndpoint *udpEndpoint = new UDPEndpoint();
            udpEndpoint->from_json(polymorphicJson);
            endpoint = udpEndpoint;
        }
    }
}

void to_json(nlohmann::json &j, const ConsumerMetadata &consumerMetadata)
{
    consumerMetadata.to_json(j);
}

bool ConsumerMetadata::operator<(const ConsumerMetadata &other) const
{
    return id < other.getId();
}

bool ConsumerMetadata::operator==(const ConsumerMetadata &other) const
{
    return id == other.getId();
}
