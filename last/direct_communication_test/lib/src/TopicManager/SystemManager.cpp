#include "SystemManager.hpp"

SystemManager::SystemManager(const CommunicationType c, const Logger &l, BrokerMetadata b) : bootstrapBroker(b), communicationType(c), logger(l)
{
    createCommunication();
    init();
}

SystemManager::~SystemManager()
{
    delete sourceEndpoint;
    delete communication;
}

void SystemManager::createCommunication()
{
    sourceEndpoint = EndpointFactory::createEndpoint(communicationType);
    communication = CommunicationFactory::createCommunication(communicationType, *sourceEndpoint, logger);
}

void SystemManager::init()
{
    json requestJson;
    requestJson["operation"] = "getClusterInformation";
    std::string requestString = requestJson.dump();

    char response[1024];
    CommunicationUtils::request(communication, communicationType, bootstrapBroker.getEndpoint(),
                                requestString.c_str(), requestString.size(), response, sizeof(response));
    logger.log("[System Manager] Received response: %s", response);

    json j = json::parse(response);
    clusterMetadata.from_json(j);
}

int SystemManager::askForID()
{
    json requestJson;
    requestJson["operation"] = "askForID";
    std::string requestString = requestJson.dump();

    char response[1024];
    CommunicationUtils::request(communication, communicationType, bootstrapBroker.getEndpoint(),
                                requestString.c_str(), requestString.size(), response, sizeof(response));

    json j = json::parse(response);

    return j["ID"].get<int>();
}

ClusterMetadata SystemManager::getClusterMetadata() const
{
    return clusterMetadata;
}
