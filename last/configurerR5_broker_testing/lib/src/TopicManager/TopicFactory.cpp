#include "TopicFactory.hpp"

TopicFactory::TopicFactory(CommunicationType commType, const Logger &l, BrokerMetadata bootstrapBroker) : communicationType(commType), logger(l), systemManager(communicationType, logger, bootstrapBroker)
{
    retrieveClusterInformation();
    createTopics();
}

TopicFactory::TopicFactory(CommunicationType commType, const Logger &l) : communicationType(commType), logger(l), systemManager(communicationType, logger)
{
    retrieveClusterInformation();
    createTopics();
}

void TopicFactory::retrieveClusterInformation()
{
    clusterMetadata = systemManager.getClusterMetadata();
}

int TopicFactory::askForID()
{
    return systemManager.askForID();
}

void TopicFactory::createTopics()
{
    for (BrokerMetadata brokerMetadata : clusterMetadata.getBrokersMetadata())
    {
        logger.log("[Topic Factory] Creating the broker instance in the TopicFactory for broker with the following endpoint info:");
        brokerMetadata.getEndpoint()->printEndpointInformation(logger);

        logger.log("[Topic Factory] In this broker we have the following topics:");

        for (TopicMetadata topicMetadata : brokerMetadata.getTopicsMetadata())
        {
            TopicProxy *topicProxy = new TopicProxy(communicationType, brokerMetadata, topicMetadata, logger);
            logger.log("%s", topicProxy->getTopicMetadata().getName().c_str());
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
