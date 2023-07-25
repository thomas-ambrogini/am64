#include "RealTopic.hpp"

RealTopic::RealTopic(TopicMetadata t, const Logger &l) : Topic(t, l)
{
}

RealTopic::~RealTopic()
{
}

void RealTopic::publish(ProducerMetadata producerMetadata, Record record)
{
    records.push_back(record);
}

void RealTopic::subscribe(ConsumerMetadata consumerMetadata)
{
    consumers.insert(std::make_pair(consumerMetadata, 0));
}

void RealTopic::unsubscribe(ConsumerMetadata consumerMetadata)
{
    consumers.erase(consumerMetadata);
}

Record RealTopic::poll(ConsumerMetadata consumerMetadata)
{
    int consumerOffset = consumers[consumerMetadata];

    if (consumerOffset < static_cast<int>(records.size()))
    {
        consumers[consumerMetadata]++;
        return records[consumerOffset];
    }
    else
    {
        return Record();
    }
}

std::map<ConsumerMetadata, int> RealTopic::getConsumers() const
{
    return consumers;
}

int RealTopic::read(ConsumerMetadata cons) {
    return cons.getId();
}