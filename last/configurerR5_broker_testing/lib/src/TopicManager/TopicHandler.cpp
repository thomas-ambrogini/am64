#include "TopicHandler.hpp"

TopicHandler::TopicHandler(const CommunicationType commType, const Logger &l, Communication *comm) : communicationType(commType), logger(l), communication(comm)
{
    logger.log("Communicatin Type: %d", communicationType);
}

TopicHandler::~TopicHandler()
{
    delete communication;
}

void TopicHandler::save(Record record, TopicMetadata topicMetadata, ProducerMetadata producerMetadata)
{
    int topicIndex = findTopicIndex(topicMetadata);

    if (topicIndex != -1)
    {
        //topics[topicIndex].publish(producerMetadata, record);

        for (const auto& pair : topics[topicIndex].getConsumers()) {
            const ConsumerMetadata cons = pair.first;
            logger.log("Sending the message to:");
            cons.getEndpoint()->printEndpointInformation(logger);
            json recordJSON;
            record.to_json(recordJSON);
            communication->write(recordJSON.dump().c_str(), recordJSON.dump().size() + 1, *cons.getEndpoint());
        }

    } 
}

void TopicHandler::subscribe(ConsumerMetadata consumerMetadata, TopicMetadata topicMetadata)
{
    logger.log("[Topic Handler] The messages will be delivered to:");
    consumerMetadata.getEndpoint()->printEndpointInformation(logger);

    int topicIndex = findTopicIndex(topicMetadata);
    if (topicIndex != -1)
    {
        topics[topicIndex].subscribe(consumerMetadata);
    }
}

void TopicHandler::unsubscribe(ConsumerMetadata consumerMetadata, TopicMetadata topicMetadata)
{
    int topicIndex = findTopicIndex(topicMetadata);
    if (topicIndex != -1)
    {
        topics[topicIndex].unsubscribe(consumerMetadata);
    }
}

void TopicHandler::poll(ConsumerMetadata consumerMetadata, TopicMetadata topicMetadata)
{

    int topicIndex = findTopicIndex(topicMetadata);
    if (topicIndex != -1)
    {
        Record record = topics[topicIndex].poll(consumerMetadata);
        json recordJSON;
        record.to_json(recordJSON);

        communication->write(recordJSON.dump().c_str(), recordJSON.dump().size() + 1, *consumerMetadata.getEndpoint());
    }
}

int TopicHandler::findTopicIndex(TopicMetadata topicMetadata)
{
    for (size_t i = 0; i < topics.size(); ++i)
    {
        if (topics[i].getTopicMetadata().getName() == topicMetadata.getName())
        {
            return i;
        }
    }

    return -1;
}

void TopicHandler::addHandledTopic(TopicMetadata topicMetadata)
{
    RealTopic topic(topicMetadata, logger);
    topics.push_back(topic);
}
