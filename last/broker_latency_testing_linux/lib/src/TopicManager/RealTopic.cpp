#include "RealTopic.hpp"

RealTopic::RealTopic(TopicMetadata t, const Logger &l) : Topic(t, l)
{
}

RealTopic::~RealTopic()
{
}

void RealTopic::publish(ProducerMetadata producerMetadata, Record record)
{
    logger.log("[Real Topic] The producer %d added the following record: %s", producerMetadata.getId(), record.getData().c_str());
    if (recordCount < maxRecords) {
        records[recordCount++] = new Record(std::move(record));
    }
}

void RealTopic::subscribe(ConsumerMetadata consumerMetadata)
{
    logger.log("[Real Topic] The consumer %d has subscribed to the topic: %s", consumerMetadata.getId(), topicMetadata.getName().c_str());
    consumers.insert(std::make_pair(consumerMetadata, 0));
    logger.log("[Real Topic] Number of subscribers: %d", consumers.size());
}

void RealTopic::unsubscribe(ConsumerMetadata consumerMetadata)
{
    logger.log("[Real Topic] The consumer %d has unsubscribed to the topic: %s", consumerMetadata.getId(), topicMetadata.getName().c_str());
    consumers.erase(consumerMetadata);
    logger.log("[Real Topic] Number of subscribers remained: %d", consumers.size());
}

Record RealTopic::poll(ConsumerMetadata consumerMetadata)
{
    logger.log("[Real Topic] The consumer %d wants to read a record from the topic: %s", consumerMetadata.getId(), topicMetadata.getName().c_str());

    int consumerOffset = consumers[consumerMetadata];
    logger.log("[Real Topic] Offset of the record to read: %d", consumerOffset);

    if (consumerOffset < recordCount)
    {
        consumers[consumerMetadata]++;
        logger.log("[Real Topic] Next offset to read: %d", consumers[consumerMetadata]);
        return *(records[consumerOffset]);
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
