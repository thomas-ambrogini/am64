#ifndef BROKER_H
#define BROKER_H

#include "BrokerMetadata.hpp"
#include "Endpoint.hpp"
#include "CommunicationType.hpp"
#include "CommunicationFactory.hpp"
#include "EndpointFactory.hpp"
#include "ProducerMetadata.hpp"
#include "Record.hpp"
#include "ConsumerMetadata.hpp"
#include "TopicHandler.hpp"
#include "CommunicationUtils.hpp"
#include "Message.hpp"

class Broker
{
public:
    Broker(enum CommunicationType commType, const Endpoint &endpoint, const Logger &logger, BrokerMetadata configurerMetadata, const bool testing, const bool push);
    Broker(enum CommunicationType commType, const Endpoint &endpoint, const Logger &logger, BrokerMetadata configurerMetadata, const bool testing);

    ~Broker();

    void start();
    void printHandledTopics();

    bool getMode() const;

private:
    BrokerMetadata brokerMetadata;
    BrokerMetadata configurerMetadata;
    const CommunicationType communicationType;
    const Logger &logger;
    Communication *communication;
    TopicHandler topicHandler;
    const bool testing;
    const bool push;

    void handleOperation(const char *request, Endpoint *sourceEndpoint);
    void askForTopics();
    void createTestingTopic();
};

#endif
