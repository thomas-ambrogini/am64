#include "Producer.hpp"

Producer::Producer(const CommunicationType c, const Logger &l, BrokerMetadata bootstrapBroker) : logger(l), topicFactory(c, logger, bootstrapBroker)
{
    askForID();
}

Producer::Producer(const CommunicationType c, const Logger &l) : logger(l), topicFactory(c, logger, 'p')
{
    producerMetadata = ProducerMetadata(1);
}

void Producer::askForID()
{
    int id = topicFactory.askForID();
    producerMetadata = ProducerMetadata(id);
}

void Producer::publish(ProducerRecord producerRecord)
{
    Topic *topic = topicFactory.getTopic(producerRecord.getTopicMetadata());
    topic->publish(producerMetadata, producerRecord.getRecord());
}
