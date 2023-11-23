#include "Consumer.hpp"

Consumer::Consumer(const CommunicationType commType, const Logger &l, BrokerMetadata bootstrapBroker) : logger(l), topicFactory(commType, logger, bootstrapBroker)
{
    askForID();
}

void Consumer::askForID()
{
    int id = topicFactory.askForID();
    consumerMetadata = ConsumerMetadata(id, nullptr);
}

void Consumer::subscribe(TopicMetadata topicMetadata)
{
    Topic *topic = topicFactory.getTopic(topicMetadata);
    subscribedTopics.push_back(topicMetadata);
    topic->subscribe(consumerMetadata);
}

void Consumer::unsubscribe(TopicMetadata topicMetadata)
{
    Topic *topic = topicFactory.getTopic(topicMetadata);
    topic->unsubscribe(consumerMetadata);
}

Record Consumer::poll(TopicMetadata topicMetadata)
{
    Topic *topic = topicFactory.getTopic(topicMetadata);
    return topic->poll(consumerMetadata);
}

std::vector<TopicMetadata> Consumer::listSubscribedTopics()
{
    return subscribedTopics;
}
