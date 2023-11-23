#ifndef CLUSTERMETADATA_H
#define CLUSTERMETADATA_H

#include "BrokerMetadata.hpp"
#include "json.hpp"
#include <iostream>
#include <string>
#include <vector>

using json = nlohmann::json;

class ClusterMetadata
{
public:
    ~ClusterMetadata();

    std::vector<BrokerMetadata> getBrokersMetadata();
    void addBrokerMetadata(const BrokerMetadata b);

    void to_json(json &j) const;
    void from_json(const json &j);

private:
    std::vector<BrokerMetadata> brokersMetadata;
};

void to_json(json &j, const ClusterMetadata &obj);

#endif
