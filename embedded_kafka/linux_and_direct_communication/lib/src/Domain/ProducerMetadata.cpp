#include "ProducerMetadata.hpp"

ProducerMetadata::ProducerMetadata() {}

ProducerMetadata::ProducerMetadata(int value) : id(value) {}

int ProducerMetadata::getId() const
{
    return id;
}

void ProducerMetadata::to_json(json &j) const
{
    j = json{{"id", id}};
}

void ProducerMetadata::from_json(const json &j)
{
    id = j.at("id").get<int>();
}

void to_json(nlohmann::json &j, const ProducerMetadata &producerMetadata)
{
    producerMetadata.to_json(j);
}
