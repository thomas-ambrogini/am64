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
    ClientMetadata clientMetadata(consumerMetadata.getId());
   
    Message message;
    message.operation = "s";
    message.clientMetadata = clientMetadata;
    message.topicMetadata = topicMetadata;

    std::string serializedMessage = SerializeMessage(message);

    logger.log("[Topic Proxy] Sending the following message: %s", serializedMessage.c_str());

    communication->write(serializedMessage.c_str(), serializedMessage.size() + 1, *brokerMetadata.getEndpoint());
}

void TopicProxy::unsubscribe(ConsumerMetadata consumerMetadata)
{
    ClientMetadata clientMetadata(consumerMetadata.getId());
   
    Message message;
    message.operation = "u";
    message.clientMetadata = clientMetadata;
    message.topicMetadata = topicMetadata;

    std::string serializedMessage = SerializeMessage(message);

    logger.log("[Topic Proxy] Sending the following message: %s", serializedMessage.c_str());

    communication->write(serializedMessage.c_str(), serializedMessage.size() + 1, *brokerMetadata.getEndpoint());
}

Record TopicProxy::poll(ConsumerMetadata consumerMetadata)
{
    ClientMetadata clientMetadata(consumerMetadata.getId());
   
    Message message;
    message.operation = "a";
    message.clientMetadata = clientMetadata;
    message.topicMetadata = topicMetadata;

    std::string serializedMessage = SerializeMessage(message);
    logger.log("[Topic Proxy] Sending the following message: %s", serializedMessage.c_str());

    communication->write(serializedMessage.c_str(), serializedMessage.size() + 1, *brokerMetadata.getEndpoint());

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
    int result = communication->read(response, sizeof(response), *brokerMetadata.getEndpoint());

    logger.log("[Topic Proxy read] Message read: %s", response);

    return result;
}