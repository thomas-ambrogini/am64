#ifndef PRODUCERRECORD_H
#define PRODUCERRECORD_H

#include "Record.hpp"
#include "TopicMetadata.hpp"

class ProducerRecord
{
public:
    ProducerRecord(TopicMetadata topicMetadata, Record record);

    TopicMetadata getTopicMetadata() const;
    Record getRecord() const;

private:
    TopicMetadata topicMetadata;
    Record record;
};

#endif
