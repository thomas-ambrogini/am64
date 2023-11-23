#ifndef CONFIGURER_H
#define CONFIGURER_H

#include "ClusterMetadata.hpp"
#include "BrokerMetadata.hpp"
#include "CommunicationFactory.hpp"
#include "Communication.hpp"
#include "EndpointFactory.hpp"
#include "Logger.hpp"
#include "json.hpp"
#include "JsonUtils.hpp"
#include "CommunicationUtils.hpp"

#include <atomic>

using json = nlohmann::json;

class Configurer
{
public:
    Configurer(CommunicationType commType, const Endpoint &endpoint, const Logger &l);
    Configurer(CommunicationType commType, const Endpoint &endpoint, const Logger &l, const std::string configFile, const bool testing);

    ~Configurer();

    void start();

private:
    ClusterMetadata clusterMetadata;
    std::vector<TopicMetadata> topics;
    CommunicationType communicationType;
    const Logger &logger;
    Communication *communication;
    int numberOfBrokers;
    std::vector<Endpoint *> brokerEndpoints;
    bool initOver;
    BrokerMetadata linuxConfigurerMetadata;
    const bool testing;

    const std::string configFile = "../conf/configFile.json";
    const std::string topicsFile = "build/conf/topics.json";
    std::atomic<int> counter;

    void retrieveClusterInformation();
    void retrieveTopics();
    void handleOperation(const char *request, Endpoint *sourceEndpoint);
    void checkInit();
    void initCommunication();
};

#endif
