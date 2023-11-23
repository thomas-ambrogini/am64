#include "TopicProxy.hpp"

TopicProxy::TopicProxy(CommunicationType type, BrokerMetadata bm, TopicMetadata t, const Logger &l) : Topic(t, l), communicationType(type), brokerMetadata(bm)
{
    Endpoint *sourceEndpoint = EndpointFactory::createEndpoint(communicationType);
    logger.log("[Topic Proxy] Endpoint of the TopicProxy: ");
    sourceEndpoint->printEndpointInformation(logger);
    communication = CommunicationFactory::createCommunication(communicationType, *sourceEndpoint, logger);
}

TopicProxy::~TopicProxy()
{
}

void TopicProxy::publish(ProducerMetadata producerMetadata, Record record)
{
    // json request;
    // request["op"] = "p";
    // request["pm"] = producerMetadata;
    // request["tm"] = topicMetadata;
    // request["r"] = record.getData();

    // std::string requestString = request.dump();
    ClientMetadata clientMetadata(producerMetadata.getId());

    Message message;
    message.operation = "p";
    message.clientMetadata = clientMetadata;
    message.record = record;
    message.topicMetadata = topicMetadata;

    std::string serializedMessage = SerializeMessage(message);
    logger.log("[Topic Proxy] Sending the following message: %s", serializedMessage.c_str());

    communication->write(serializedMessage.c_str(), serializedMessage.size() + 1, *brokerMetadata.getEndpoint());
}

void TopicProxy::subscribe(ConsumerMetadata consumerMetadata)
{
    json request;
    request["op"] = "s";
    request["cm"] = consumerMetadata;
    request["tm"] = topicMetadata;

    std::string requestString = request.dump();
    logger.log("[Topic Proxy] Sending the following message: %s", requestString.c_str());

    communication->write(requestString.c_str(), requestString.size() + 1, *brokerMetadata.getEndpoint());
}

void TopicProxy::unsubscribe(ConsumerMetadata consumerMetadata)
{
    json request;
    request["op"] = "u";
    request["cm"] = consumerMetadata;
    request["tm"] = topicMetadata;

    std::string requestString = request.dump();
    logger.log("[Topic Proxy] Sending the following message: %s", requestString.c_str());

    communication->write(requestString.c_str(), requestString.size() + 1, *brokerMetadata.getEndpoint());
}

Record TopicProxy::poll(ConsumerMetadata consumerMetadata)
{
    json request;
    request["op"] = "a";
    request["cm"] = consumerMetadata;
    request["tm"] = topicMetadata;

    std::string requestString = request.dump();
    logger.log("[Topic Proxy] Sending the following message: %s", requestString.c_str());

    communication->write(requestString.c_str(), requestString.size() + 1, *brokerMetadata.getEndpoint());

    char response[1024];

    if (communication->read(response, sizeof(response), *brokerMetadata.getEndpoint()) < 0)
    {
        logger.logError("[Topic Proxy] Failed to receive message from client");
    }

    logger.log("[Topic Proxy] Response received from the broker: %s", response);

    nlohmann::json deserializedResponse = nlohmann::json::parse(response);
    Record record;
    record.from_json(deserializedResponse);

    return record;
}

int TopicProxy::read(ConsumerMetadata consumerMetadata){
    char response[1024];
    return communication->read(response, sizeof(response), *brokerMetadata.getEndpoint());
}