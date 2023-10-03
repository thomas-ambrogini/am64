#include "ProducerRecord.hpp"

ProducerRecord::ProducerRecord(TopicMetadata t, Record r) : topicMetadata(t), record(r)
{
}

TopicMetadata ProducerRecord::getTopicMetadata() const
{
    return topicMetadata;
}

Record ProducerRecord::getRecord() const
{
    return record;
}
