#ifndef TOPICPROXY_H
#define TOPICPROXY_H

#include "Topic.hpp"
#include "Record.hpp"
#include "BrokerMetadata.hpp"
#include "Communication.hpp"
#include "CommunicationFactory.hpp"
#include "CommunicationType.hpp"
#include "EndpointFactory.hpp"
#include "json.hpp"

using json = nlohmann::json;

class TopicProxy : public Topic
{
public:
    TopicProxy(CommunicationType communicationType, TopicMetadata topicMetadata, const Logger &logger);
    TopicProxy(CommunicationType communicationType, BrokerMetadata brokerMetadata, TopicMetadata topicMetadata, const Logger &logger);

    ~TopicProxy();

    void publish(ProducerMetadata producerMetadata, Record record) override;
    void subscribe(ConsumerMetadata consumerMetadata) override;
    void unsubscribe(ConsumerMetadata consumerMetadata) override;
    Record poll(ConsumerMetadata consumerMetadata) override;

private:
    const CommunicationType communicationType;
    BrokerMetadata brokerMetadata;
    Communication *communication;

    int findConsumer(int consumerId);
};

#endif
