#ifndef TOPICMETADATA_H
#define TOPICMETADATA_H

#include "json.hpp"

using json = nlohmann::json;

class TopicMetadata
{
public:
    TopicMetadata();
    TopicMetadata(const std::string topicName);

    const std::string &getName() const;

    void to_json(json &j) const;
    void from_json(const json &j);

private:
    std::string name;
};

void to_json(nlohmann::json &j, const TopicMetadata &obj);

#endif
