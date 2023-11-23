#include "Broker.hpp"

Broker::Broker(CommunicationType commType, const Endpoint &endpoint, const Logger &l, BrokerMetadata configurerMetadata, const bool t, const bool p) : brokerMetadata(const_cast<Endpoint *>(&endpoint)),
                                                                                                                           configurerMetadata(configurerMetadata),
                                                                                                                           communicationType(commType),
                                                                                                                           logger(l),
                                                                                                                           communication(CommunicationFactory::createCommunication(commType, endpoint, logger)),
                                                                                                                           topicHandler(communicationType, logger, communication, p),
                                                                                                                           testing(t),
                                                                                                                           push(p)
{
    if(!testing) {
        askForTopics();
    }else {
        createTestingTopic();
    }
}

Broker::Broker(CommunicationType commType, const Endpoint &endpoint, const Logger &l, BrokerMetadata configurerMetadata, const bool t) : Broker(commType, endpoint, l, configurerMetadata, t, false)
{
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
        logger.log("[Broker] Request received from the client: %s with size %d", clientRequest, strlen(clientRequest));
        sourceEndpoint->printEndpointInformation(logger);

        handleOperation(clientRequest, sourceEndpoint);
    }

    delete sourceEndpoint;
}

void Broker::handleOperation(const char *request, Endpoint *sourceEndpoint)
{
    Message deserializedMessage = DeserializeMessage(request);

    if (deserializedMessage.operation == "p")
    {
        logger.log("Handling the publish of a message");
        logger.log("Operation: %s", deserializedMessage.operation.c_str());
        logger.log("ID: %d", deserializedMessage.clientMetadata.getId());
        logger.log("Record: %s", deserializedMessage.record.getData().c_str());
        logger.log("Topic Name: %s", deserializedMessage.topicMetadata.getName().c_str());

        topicHandler.save(deserializedMessage.record, deserializedMessage.topicMetadata, ProducerMetadata(deserializedMessage.clientMetadata.getId()));
    }
    else if(deserializedMessage.operation == "s"){
        logger.log("Handling subscribe message");
        ConsumerMetadata consumerMetadata(deserializedMessage.clientMetadata.getId(), nullptr);
        if (consumerMetadata.getEndpoint() == nullptr)
        {
            Endpoint * consumerEndpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
            static_cast<RPMessageEndpoint *>(consumerEndpoint)->setCoreId(static_cast<RPMessageEndpoint *>(sourceEndpoint)->getCoreId());
            static_cast<RPMessageEndpoint *>(consumerEndpoint)->setServiceEndpoint(static_cast<RPMessageEndpoint *>(sourceEndpoint)->getServiceEndpoint()); 
            consumerMetadata.setEndpoint(consumerEndpoint);
        }

        topicHandler.subscribe(consumerMetadata, deserializedMessage.topicMetadata);
    }
    else if (deserializedMessage.operation == "u")
    {
        logger.log("Handling unsubscribe message");
        ConsumerMetadata consumerMetadata(deserializedMessage.clientMetadata.getId(), nullptr);

        topicHandler.unsubscribe(consumerMetadata, deserializedMessage.topicMetadata);
    }
    else if (deserializedMessage.operation == "a")
    {
        logger.log("Handling poll message");
        ConsumerMetadata consumerMetadata(deserializedMessage.clientMetadata.getId(), nullptr);
        if (consumerMetadata.getEndpoint() == nullptr)
        {
            Endpoint * consumerEndpoint = EndpointFactory::createEndpoint(CommunicationType::RPMessage);
            static_cast<RPMessageEndpoint *>(consumerEndpoint)->setCoreId(static_cast<RPMessageEndpoint *>(sourceEndpoint)->getCoreId());
            static_cast<RPMessageEndpoint *>(consumerEndpoint)->setServiceEndpoint(static_cast<RPMessageEndpoint *>(sourceEndpoint)->getServiceEndpoint()); 
            consumerMetadata.setEndpoint(consumerEndpoint); 
        }

        topicHandler.poll(consumerMetadata, deserializedMessage.topicMetadata);
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

void Broker::createTestingTopic() {
    TopicMetadata topicMetadata("Testing");
    topicHandler.addHandledTopic(topicMetadata);
}

void Broker::printHandledTopics() 
{
    topicHandler.printHandledTopics();
}

bool Broker::getMode() const {
    return push;
}
