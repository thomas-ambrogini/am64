#include "Broker.hpp"

Broker::Broker(CommunicationType commType, const Endpoint &endpoint, const Logger &l, BrokerMetadata configurerMetadata) : brokerMetadata(const_cast<Endpoint *>(&endpoint)),
                                                                                                                           configurerMetadata(configurerMetadata),
                                                                                                                           communicationType(commType),
                                                                                                                           logger(l),
                                                                                                                           communication(CommunicationFactory::createCommunication(commType, endpoint, logger)),
                                                                                                                           topicHandler(communicationType, logger, communication)
{
    askForTopics();
}

Broker::Broker(CommunicationType commType, const Endpoint &endpoint, const Logger &l) : brokerMetadata(const_cast<Endpoint *>(&endpoint)),
                                                                                        communicationType(commType),
                                                                                        logger(l),
                                                                                        communication(CommunicationFactory::createCommunication(commType, endpoint, logger)),
                                                                                        topicHandler(communicationType, logger, communication)
{
    createTopics();
}

Broker::~Broker()
{
    delete communication;
}

void Broker::start()
{
    char clientRequest[1024];
    Endpoint *sourceEndpoint = EndpointFactory::createEndpoint(communicationType);

    while (true)
    {
        if (communication->read(clientRequest, sizeof(clientRequest), *sourceEndpoint) < 0)
        {
            logger.logError("[Broker] Failed to receive message from client");
            break;
        }

        handleOperation(clientRequest, sourceEndpoint);
    }

    delete sourceEndpoint;
}

void Broker::handleOperation(const char *request, Endpoint *sourceEndpoint)
{
    nlohmann::json deserializedRequest = nlohmann::json::parse(request);
    std::string operation = deserializedRequest["operation"];

    if (operation == "publish")
    {
        json jsonProducerMetadata = deserializedRequest["producerMetadata"];
        ProducerMetadata producerMetadata;
        producerMetadata.from_json(jsonProducerMetadata);

        std::string data = deserializedRequest["record"];
        Record record(data);

        json jsonTopicMetadata = deserializedRequest["topicMetadata"];
        TopicMetadata topicMetadata;
        topicMetadata.from_json(jsonTopicMetadata);

        topicHandler.save(record, topicMetadata, producerMetadata);
        // topicHandler.updateConsumers(topicMetadata);
    }
    else if (operation == "subscribe")
    {
        json jsonConsumerMetadata = deserializedRequest["consumerMetadata"];
        ConsumerMetadata consumerMetadata;
        consumerMetadata.from_json(jsonConsumerMetadata);
        if (consumerMetadata.getEndpoint() == nullptr)
        {
            Endpoint * consumerEndpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
            static_cast<RPMessageEndpoint *>(consumerEndpoint)->setCoreId(static_cast<RPMessageEndpoint *>(sourceEndpoint)->getCoreId());
            static_cast<RPMessageEndpoint *>(consumerEndpoint)->setServiceEndpoint(static_cast<RPMessageEndpoint *>(sourceEndpoint)->getServiceEndpoint()); 
            consumerMetadata.setEndpoint(consumerEndpoint);
        }

        json jsonTopicMetadata = deserializedRequest["topicMetadata"];
        TopicMetadata topicMetadata;
        topicMetadata.from_json(jsonTopicMetadata);

        logger.log("[Broker] The consumer with ID: %d wants to subscribe on the topic: %s", consumerMetadata.getId(), topicMetadata.getName().c_str());

        topicHandler.subscribe(consumerMetadata, topicMetadata);
    }
    else if (operation == "unsubscribe")
    {
        json jsonConsumerMetadata = deserializedRequest["consumerMetadata"];
        ConsumerMetadata consumerMetadata;
        consumerMetadata.from_json(jsonConsumerMetadata);

        json jsonTopicMetadata = deserializedRequest["topicMetadata"];
        TopicMetadata topicMetadata;
        topicMetadata.from_json(jsonTopicMetadata);

        logger.log("[Broker] The consumer with ID: %d wants to unsubscribe from the topic: %s", consumerMetadata.getId(), topicMetadata.getName().c_str());

        topicHandler.unsubscribe(consumerMetadata, topicMetadata);
    }
    else if (operation == "poll")
    {
        json jsonConsumerMetadata = deserializedRequest["consumerMetadata"];
        ConsumerMetadata consumerMetadata;
        consumerMetadata.from_json(jsonConsumerMetadata);
        if (consumerMetadata.getEndpoint() == nullptr)
        {
            consumerMetadata.setEndpoint(sourceEndpoint);
        }

        json jsonTopicMetadata = deserializedRequest["topicMetadata"];
        TopicMetadata topicMetadata;
        topicMetadata.from_json(jsonTopicMetadata);

        topicHandler.poll(consumerMetadata, topicMetadata);
    }
}

void Broker::askForTopics()
{
    json requestJson;
    requestJson["operation"] = "getTopics";

    json endpointJson;
    brokerMetadata.getEndpoint()->to_json(endpointJson);
    requestJson["endpoint"] = endpointJson;

    std::string requestString = requestJson.dump();

    char response[1024];
    CommunicationUtils::request(communication, communicationType, configurerMetadata.getEndpoint(),
                                requestString.c_str(), requestString.size() + 1, response, sizeof(response));

    json topicsJson = json::parse(response);

    for (const auto &topic : topicsJson)
    {
        TopicMetadata topicMetadata(topic["name"]);
        topicHandler.addHandledTopic(topicMetadata);
    }
}

void Broker::createTopics() 
{
    TopicMetadata topicMetadata("Prova");
    topicHandler.addHandledTopic(topicMetadata);

    TopicMetadata topicMetadata2("Prova2");
    topicHandler.addHandledTopic(topicMetadata2);

}

void Broker::printHandledTopic() 
{
    topicHandler.printHandledTopic();
}