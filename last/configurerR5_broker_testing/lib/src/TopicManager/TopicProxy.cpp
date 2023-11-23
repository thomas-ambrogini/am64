#include "TopicProxy.hpp"

TopicProxy::TopicProxy(const CommunicationType type, TopicMetadata t, const Logger &l) : Topic(t, l), communicationType(type), brokerMetadata(new UDPEndpoint(1235))
{
    Endpoint *sourceEndpoint = EndpointFactory::createEndpoint(communicationType);
    communication = CommunicationFactory::createCommunication(communicationType, *sourceEndpoint, logger);
}

TopicProxy::TopicProxy(CommunicationType type, BrokerMetadata bm, TopicMetadata t, const Logger &l) : Topic(t, l), communicationType(type), brokerMetadata(bm)
{
    Endpoint *sourceEndpoint = EndpointFactory::createEndpoint(communicationType);
    communication = CommunicationFactory::createCommunication(communicationType, *sourceEndpoint, logger);
}

TopicProxy::~TopicProxy()
{
}

void TopicProxy::publish(ProducerMetadata producerMetadata, Record record)
{
    json request;
    request["operation"] = "publish";
    request["producerMetadata"] = producerMetadata;
    request["topicMetadata"] = topicMetadata;
    request["record"] = record.getData();

    std::string requestString = request.dump();

    communication->write(requestString.c_str(), requestString.size() + 1, *brokerMetadata.getEndpoint());
}

void TopicProxy::subscribe(ConsumerMetadata consumerMetadata)
{
    json request;
    request["operation"] = "subscribe";
    request["consumerMetadata"] = consumerMetadata;
    request["topicMetadata"] = topicMetadata;
    
    std::string requestString = request.dump();

    communication->write(requestString.c_str(), requestString.size() + 1, *brokerMetadata.getEndpoint());
}

void TopicProxy::unsubscribe(ConsumerMetadata consumerMetadata)
{
    json request;
    request["operation"] = "unsubscribe";
    request["consumerMetadata"] = consumerMetadata;
    request["topicMetadata"] = topicMetadata;

    std::string requestString = request.dump();

    communication->write(requestString.c_str(), requestString.size() + 1, *brokerMetadata.getEndpoint());
}

Record TopicProxy::poll(ConsumerMetadata consumerMetadata)
{
    json request;
    request["operation"] = "poll";
    request["consumerMetadata"] = consumerMetadata;
    request["topicMetadata"] = topicMetadata;

    std::string requestString = request.dump();

    communication->write(requestString.c_str(), requestString.size() + 1, *brokerMetadata.getEndpoint());

    char response[1024];

    if (communication->read(response, sizeof(response), *brokerMetadata.getEndpoint()) < 0)
    {
        logger.logError("[Topic Proxy] Failed to receive message from client");
    }

    nlohmann::json deserializedResponse = nlohmann::json::parse(response);
    Record record;
    record.from_json(deserializedResponse);

    return record;
}

int TopicProxy::read(ConsumerMetadata consumerMetadata){
    char response[1024];
    return communication->read(response, sizeof(response), *brokerMetadata.getEndpoint());
}
