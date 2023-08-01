#ifndef PRODUCERMETADATA_H
#define PRODUCERMETADATA_H

#include "json.hpp"

using json = nlohmann::json;

class ProducerMetadata
{
private:
    int id;

public:
    ProducerMetadata();
    ProducerMetadata(int value);

    int getId() const;

    void to_json(json &j) const;
    void from_json(const json &j);
};

void to_json(nlohmann::json &j, const ProducerMetadata &producerMetadata);

#endif
