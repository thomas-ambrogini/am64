#ifndef RECORD_H
#define RECORD_H

#include <string>
#include "json.hpp"

using json = nlohmann::json;

class Record
{
public:
    Record();
    Record(const std::string data);

    const std::string &getData() const;
    void setData(std::string data);

    void to_json(json &j) const;
    void from_json(const json &j);

private:
    std::string data;
};

void to_json(nlohmann::json &j, const Record &obj);

#endif
