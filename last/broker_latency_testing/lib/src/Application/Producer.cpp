#include "Producer.hpp"

Producer::Producer(const CommunicationType c, const Logger &l, BrokerMetadata bootstrapBroker, const bool t) : logger(l), topicFactory(c, logger, bootstrapBroker, t), testing(t)
{
    if(!testing) {
        askForID();
    } else {
        int randomInt = rand() % 1000;
        producerMetadata = ProducerMetadata(randomInt);
    }
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



