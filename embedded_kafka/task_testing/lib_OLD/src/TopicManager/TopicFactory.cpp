#include "TopicFactory.hpp"

TopicFactory::TopicFactory(CommunicationType commType, const Logger &l, BrokerMetadata bootstrapBroker) : communicationType(commType), logger(l), systemManager(communicationType, logger, bootstrapBroker)
{
    retrieveClusterInformation();
}

TopicFactory::TopicFactory(CommunicationType commType, const Logger &l, char c) : communicationType(commType), logger(l), systemManager(communicationType, logger)
{
    Endpoint *brokerEndpoint = new RPMessageEndpoint(1, 12);
    BrokerMetadata brokerMetadata(brokerEndpoint);

    TopicMetadata topicMetadata("Prova");
    brokerMetadata.addTopicMetadata(topicMetadata);
    clusterMetadata.addBrokerMetadata(brokerMetadata);

    createTopics(c);
}

void TopicFactory::retrieveClusterInformation()
{
    clusterMetadata = systemManager.getClusterMetadata();
}

int TopicFactory::askForID()
{
    return systemManager.askForID();
}

void TopicFactory::createTopics(char c)
{
    for (BrokerMetadata brokerMetadata : clusterMetadata.getBrokersMetadata())
    {
        logger.log("[Topic Factory] Creating the broker instance in the TopicFactory for broker with the following endpoint info:");
        brokerMetadata.getEndpoint()->printEndpointInformation(logger);

        logger.log("[Topic Factory] In this broker we have the following topics:");

        for (TopicMetadata topicMetadata : brokerMetadata.getTopicsMetadata())
        {
            logger.log("%s", topicMetadata.getName().c_str());
            TopicProxy *topicProxy = new TopicProxy(communicationType, brokerMetadata, topicMetadata, logger, c);
            topics.push_back(topicProxy);
        }
    }
}

TopicFactory::~TopicFactory()
{
}

Topic *TopicFactory::getTopic(TopicMetadata topicMetadata)
{
    int topicIndex = findTopic(topicMetadata);
    if (topicIndex != -1)
        return topics[topicIndex];
    else
        return nullptr;
}

int TopicFactory::findTopic(TopicMetadata topicMetadata)
{
    for (size_t i = 0; i < topics.size(); i++)
    {
        if (topics[i]->getTopicMetadata().getName() == topicMetadata.getName())
        {
            return i;
        }
    }

    return -1;
}
