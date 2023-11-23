#include "Configurer.hpp"

Configurer::Configurer(CommunicationType commType, const Endpoint &endpoint, const Logger &l, const std::string configFile, const bool testing) : communicationType(commType), logger(l), numberOfBrokers(0), initOver(false), configFile(configFile), counter(1), testing(testing)
{
    communication = CommunicationFactory::createCommunication(communicationType, endpoint, logger);
}

Configurer::Configurer(CommunicationType commType, const Endpoint &endpoint, const Logger &l) : Configurer(commType, endpoint, l, "../conf/configFile.json", true)
{
}

Configurer::~Configurer()
{
    delete communication;
}

void Configurer::start()
{
    initCommunication();
    retrieveTopics();

    char clientRequest[512];
    Endpoint *sourceEndpoint = EndpointFactory::createEndpoint(communicationType);

    while (true)
    {
        if (communication->read(clientRequest, sizeof(clientRequest), *sourceEndpoint) < 0)
        {
            logger.logError("[Configurer] Failed to receive message from client");
            break;
        }

        logger.log("Request received from the client: %s", clientRequest);
        sourceEndpoint->printEndpointInformation(logger);

        handleOperation(clientRequest, sourceEndpoint);
    }

    delete sourceEndpoint;
}

void Configurer::handleOperation(const char *request, Endpoint *sourceEndpoint)
{
    nlohmann::json deserializedRequest = nlohmann::json::parse(request);
    std::string operation = deserializedRequest["operation"];

    logger.log("[Configurer] Operation Received: %s", operation.c_str());

    if (operation == "getClusterInformation")
    {
        checkInit();
        json clusterJson;
        clusterMetadata.to_json(clusterJson);

        communication->write(clusterJson.dump().c_str(), clusterJson.dump().size() + 1, *sourceEndpoint);
    }
    else if (operation == "askForID")
    {
        checkInit();
        json idJson;
        idJson["ID"] = counter.fetch_add(1);

        communication->write(idJson.dump().c_str(), idJson.dump().size() + 1, *sourceEndpoint);
    }
    else if (operation == "getTopics")
    {
        json topicJson;
        for (const auto &topic : topics)
        {
            json jsonObject;
            topic.to_json(jsonObject);
            topicJson.push_back(jsonObject);
        }
        communication->write(topicJson.dump().c_str(), topicJson.dump().size() + 1, *sourceEndpoint);
    }
    else if (operation == "initTopics")
    {
        if (!initOver)
        {
            Endpoint *endpoint = EndpointFactory::createEndpoint(communicationType);
            endpoint->from_json(deserializedRequest["endpoint"]);
            brokerEndpoints.push_back(endpoint);
            numberOfBrokers++;
        }
    }

    logger.log("status of the cluster:");
    for (const auto &broker : clusterMetadata.getBrokersMetadata())
    {
        broker.getEndpoint()->printEndpointInformation(logger);
        logger.log("[Configurer] handles the topics:");

        for (const auto &topic : broker.getTopicsMetadata())
        {
            logger.log("[Configurer] Topic name: %s", topic.getName().c_str());
        }
    }
}

void Configurer::checkInit()
{
    if (initOver)
    {
        return;
    }

    initOver = true;
    int numberOfTopics = topics.size();
    int numberOfTopicsPerBroker = numberOfTopics / numberOfBrokers;
    int remainingTopics = numberOfTopics % numberOfBrokers;
    int startIndex, endIndex = 0;

    if (numberOfTopicsPerBroker == 0)
    {
        for (int i = 0; i < numberOfTopics; i++)
        {
            json topicArray;
            BrokerMetadata broker(brokerEndpoints[i]);
            TopicMetadata topic = topics[i];
            json topicJson;
            topic.to_json(topicJson);
            topicArray.push_back(topicJson);

            broker.addTopicMetadata(topic);
            clusterMetadata.addBrokerMetadata(broker);
            communication->write(topicArray.dump().c_str(), topicArray.dump().size() + 1, *brokerEndpoints[i]);
        }

        return;
    }

    for (int i = 0; i < numberOfBrokers; i++)
    {
        BrokerMetadata broker(brokerEndpoints[i]);

        if (remainingTopics > 0)
        {
            startIndex = endIndex;
            endIndex = endIndex + (numberOfTopicsPerBroker) + 1;
            remainingTopics--;
        }
        else
        {
            startIndex = endIndex;
            endIndex = endIndex + numberOfTopicsPerBroker;
        }

        std::vector<TopicMetadata> extractedElements(topics.begin() + startIndex, topics.begin() + endIndex);

        json topicJson;
        for (const auto &topic : extractedElements)
        {
            broker.addTopicMetadata(topic);
            json jsonObject;
            topic.to_json(jsonObject);
            topicJson.push_back(jsonObject);
        }
        clusterMetadata.addBrokerMetadata(broker);
        communication->write(topicJson.dump().c_str(), topicJson.dump().size() + 1, *brokerEndpoints[i]);
    }
}

void Configurer::retrieveTopics()
{
    if (communicationType == CommunicationType::RPMessage)
    {
        if (testing)
        {
            TopicMetadata topicMetadata("Testing");
            topics.push_back(topicMetadata);

            return;
        }

        json requestJson;
        requestJson["operation"] = "getTopics";
        std::string requestString = requestJson.dump();

        char response[512];
        CommunicationUtils::request(communication, communicationType, linuxConfigurerMetadata.getEndpoint(),
                                    requestString.c_str(), requestString.size() + 1, response, sizeof(response));

        json topicsJson = json::parse(response);

        for (const auto &topic : topicsJson)
        {
            TopicMetadata topicMetadata(topic["name"]);
            topics.push_back(topicMetadata);
        }

        return;
    }

#ifdef __unix__
    json jsonData = JsonUtils::readJsonFile(topicsFile, logger);

    if (!jsonData.empty())
    {
        for (const auto &entry : jsonData)
        {
            TopicMetadata obj(entry["name"]);
            topics.push_back(obj);
        }
    }
    else
    {
        logger.logError("[Configurer] The topics JSON file was empty or not found");
    }
#endif
}

void Configurer::initCommunication()
{
    if (communicationType == CommunicationType::RPMessageLinux)
    {
        char clientRequest[512] = {0};
        uint16_t msgSize;
        clientRequest[10] = '\0';
        msgSize = strlen(clientRequest) + 1;

        RPMessageEndpoint *destinationEndpoint = static_cast<RPMessageEndpoint *>(EndpointFactory::createEndpoint(CommunicationType::RPMessageLinux));
        destinationEndpoint->setCoreId(2);
        destinationEndpoint->setServiceEndpoint(14);

        communication->write(clientRequest, msgSize, *destinationEndpoint);
        communication->read(clientRequest, sizeof(clientRequest), *destinationEndpoint);

        logger.log("%s", clientRequest);
    }
    else if (communicationType == CommunicationType::RPMessage)
    {
        char clientRequest[512];
        Endpoint *sourceEndpoint = EndpointFactory::createEndpoint(communicationType);
        communication->read(clientRequest, sizeof(clientRequest), *sourceEndpoint);

        const char *initDoneMessage = "init done";
        strcpy(clientRequest, initDoneMessage);

        communication->write(clientRequest, strlen(clientRequest) + 1, *sourceEndpoint);

        linuxConfigurerMetadata.setEndpoint(sourceEndpoint);
    }
}
