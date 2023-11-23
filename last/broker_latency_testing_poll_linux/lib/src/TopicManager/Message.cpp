#include "Message.hpp"


std::string SerializeMessage(const Message& message) {
    std::string serializedMessage = message.operation + ",";
    serializedMessage += std::to_string(message.clientMetadata.getId()) + ",";
    serializedMessage += message.record.getData() + ",";
    serializedMessage += message.topicMetadata.getName();
    return serializedMessage;
}


Message DeserializeMessage(const std::string& serializedMessage) {
    Message message;
    std::string copy = serializedMessage;  // Make a non-const copy of the input string

    size_t pos = copy.find(",");
    if (pos != std::string::npos) {
        message.operation = copy.substr(0, pos);
        copy.erase(0, pos + 1);
    }

    pos = copy.find(",");
    if (pos != std::string::npos) {
        message.clientMetadata.setId(std::stoi(copy.substr(0, pos)));
        copy.erase(0, pos + 1);
    }

    pos = copy.find(",");
    if (pos != std::string::npos) {
        message.record.setData( copy.substr(0, pos));
        copy.erase(0, pos + 1);
    }

    message.topicMetadata.setName(copy);

    return message;
}
