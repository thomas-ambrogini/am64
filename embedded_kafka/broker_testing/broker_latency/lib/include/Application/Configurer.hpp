#ifdef __unix__

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

#include <fstream>
#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
#include <future>

using json = nlohmann::json;

class Configurer
{
public:
    Configurer(CommunicationType commType, const Endpoint &endpoint, const Logger &l);
    Configurer(CommunicationType commType, const Endpoint &endpoint, const Logger &l, const std::string configFile);

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

    const std::string configFile = "../conf/configFile.json";
    const std::string topicsFile = "build/conf/topics.json";
    std::atomic<int> counter;

    void retrieveClusterInformation();
    void retrieveTopics();
    void handleOperation(const char *request, Endpoint *sourceEndpoint);
    void checkInit();
};

#endif
#endif
