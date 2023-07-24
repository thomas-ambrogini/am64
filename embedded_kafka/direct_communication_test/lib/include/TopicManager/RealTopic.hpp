#ifndef REALTOPIC_H
#define REALTOPIC_H

#include "Topic.hpp"
#include <vector>
#include <map>

class RealTopic : public Topic
{
public:
    RealTopic(TopicMetadata topicMetadata, const Logger &logger);

    ~RealTopic();

    void publish(ProducerMetadata producerMetadata, Record record) override;
    void subscribe(ConsumerMetadata consumerMetadata) override;
    void unsubscribe(ConsumerMetadata consumerMetadata) override;
    Record poll(ConsumerMetadata consumerMetadata) override;

    std::map<ConsumerMetadata, int> getConsumers() const;

private:
    std::vector<Record> records;
    std::map<ConsumerMetadata, int> consumers;
};

#endif
