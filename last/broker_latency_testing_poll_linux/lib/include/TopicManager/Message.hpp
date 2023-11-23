#ifndef MESSAGE_H
#define MESSAGE_H

#include "Record.hpp"
#include "ProducerMetadata.hpp"
#include "ClientMetadata.hpp"
#include "TopicMetadata.hpp"

struct Message {
    std::string operation;
    ClientMetadata clientMetadata;
    Record record;
    TopicMetadata topicMetadata;
};

std::string SerializeMessage(const Message& message);
Message DeserializeMessage(const std::string& serializedMessage);


#endif
