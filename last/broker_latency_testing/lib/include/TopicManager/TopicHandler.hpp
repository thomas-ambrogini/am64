#ifndef TOPICHANDLER_H
#define TOPICHANDLER_H

#include "Record.hpp"
#include "RealTopic.hpp"
#include "TopicMetadata.hpp"
#include "ProducerMetadata.hpp"
#include "Communication.hpp"
#include "CommunicationType.hpp"
#include "Logger.hpp"
#include "json.hpp"

using json = nlohmann::json;

class TopicHandler
{
public:
    TopicHandler(const CommunicationType communicationType, const Logger &logger, Communication *communication, const bool pull);

    ~TopicHandler();

    void save(Record record, TopicMetadata topicMetadata, ProducerMetadata producerMetadata);
    void subscribe(ConsumerMetadata consumerMetadata, TopicMetadata topicMetadata);
    void unsubscribe(ConsumerMetadata consumerMetadata, TopicMetadata topicMetadata);
    void poll(ConsumerMetadata consumerMetadata, TopicMetadata topicMetadata);

    void addHandledTopic(TopicMetadata topic);
    void printHandledTopics();

private:
    std::vector<RealTopic> topics;
    const CommunicationType communicationType;
    const Logger &logger;
    Communication *communication;

    const bool push;

    int findTopicIndex(TopicMetadata topicMetadata);
};

#endif
