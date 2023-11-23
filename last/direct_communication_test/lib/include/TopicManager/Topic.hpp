#ifndef TOPIC_H
#define TOPIC_H

#include "Record.hpp"
#include "TopicMetadata.hpp"
#include "ConsumerMetadata.hpp"
#include "ProducerMetadata.hpp"
#include "Logger.hpp"

class Topic
{
public:
    Topic(const TopicMetadata topicMetadata, const Logger &logger);

    virtual ~Topic();

    virtual void publish(ProducerMetadata producerMetadata, Record record) = 0;
    virtual void subscribe(ConsumerMetadata consumerMetadata) = 0;
    virtual void unsubscribe(ConsumerMetadata consumerMetadata) = 0;
    virtual Record poll(ConsumerMetadata consumerMetadata) = 0;

    const TopicMetadata getTopicMetadata() const;

protected:
    const TopicMetadata topicMetadata;
    const Logger &logger;
};

#endif
