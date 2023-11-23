#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "BrokerMetadata.hpp"
#include "ClusterMetadata.hpp"
#include "CommunicationFactory.hpp"
#include "Communication.hpp"
#include "CommunicationType.hpp"
#include "EndpointFactory.hpp"
#include "json.hpp"
#include "CommunicationUtils.hpp"

using json = nlohmann::json;

class SystemManager
{
public:
    SystemManager(const CommunicationType communicationType, const Logger &logger, BrokerMetadata boostrapBroker);

    ~SystemManager();

    ClusterMetadata getClusterMetadata() const;

    int askForID();

private:
    BrokerMetadata bootstrapBroker;
    ClusterMetadata clusterMetadata;

    const CommunicationType communicationType;
    const Logger &logger;

    Endpoint *sourceEndpoint;
    Communication *communication;

    void init();
    void createCommunication();
};

#endif
