#include "Consumer.hpp"

Consumer::Consumer(const CommunicationType commType, const Logger &l, BrokerMetadata bootstrapBroker, const bool t) : logger(l), topicFactory(commType, logger, bootstrapBroker, t), testing(t)
{
    if(!testing) {
        askForID();
    }else {
        int randomInt = rand() % 1000;
        consumerMetadata = ConsumerMetadata(randomInt, nullptr);
    }
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

int Consumer::read(TopicMetadata topicMetadata) {
    Topic *topic = topicFactory.getTopic(topicMetadata);
    return topic->read(consumerMetadata);
}

std::vector<TopicMetadata> Consumer::listSubscribedTopics()
{
    return subscribedTopics;
}
