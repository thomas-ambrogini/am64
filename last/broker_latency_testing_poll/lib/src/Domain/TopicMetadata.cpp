#include "TopicMetadata.hpp"

TopicMetadata::TopicMetadata()
{
}

TopicMetadata::TopicMetadata(const std::string n) : name(n)
{
}

const std::string &TopicMetadata::getName() const
{
    return name;
}

void TopicMetadata::setName(const std::string topicName) {
    name = topicName;
}


void TopicMetadata::to_json(json &j) const
{
    j = json{{"name", name}};
}

void TopicMetadata::from_json(const json &j)
{
    name = j.at("name").get<std::string>();
}

void to_json(nlohmann::json &j, const TopicMetadata &obj)
{
    obj.to_json(j);
}
