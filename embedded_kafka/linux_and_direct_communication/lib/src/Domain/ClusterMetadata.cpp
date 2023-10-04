#include "ClusterMetadata.hpp"

#include <iostream>

ClusterMetadata::~ClusterMetadata()
{
    // Delete the memory occupied by the pointer of endpoint inside the broker metadata object
}

std::vector<BrokerMetadata> ClusterMetadata::getBrokersMetadata()
{
    return brokersMetadata;
}

void ClusterMetadata::addBrokerMetadata(const BrokerMetadata b)
{
    brokersMetadata.push_back(b);
}

void ClusterMetadata::to_json(json &j) const
{
    j = json{{"brokers", brokersMetadata}};
}

void ClusterMetadata::from_json(const json &j)
{
    for (const auto &item : j.at("brokers"))
    {
        BrokerMetadata obj;
        obj.from_json(item);
        brokersMetadata.push_back(obj);
    }
}

void to_json(json &j, const ClusterMetadata &obj)
{
    obj.to_json(j);
}
