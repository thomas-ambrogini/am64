#include "SystemManager.hpp"

SystemManager::SystemManager(const CommunicationType c, const Logger &l, BrokerMetadata b, const bool t) : bootstrapBroker(b), communicationType(c), logger(l), testing(t)
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
    if(!testing) {
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
    else {
        int serviceEndpoint = -1;

        if(communicationType == RPMessageLinux) {
            //Create the information of the cluster statically. With the broker on the R500
            serviceEndpoint = 2;
        } else if (communicationType == RPMessage){
            serviceEndpoint = 1;
        }
        Endpoint *  brokerEndpoint = new RPMessageEndpoint(serviceEndpoint, 12);
        BrokerMetadata brokerMetadata(brokerEndpoint);
        
        TopicMetadata topicMetadata("Testing");
        brokerMetadata.addTopicMetadata(topicMetadata);
        clusterMetadata.addBrokerMetadata(brokerMetadata);
    }
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
