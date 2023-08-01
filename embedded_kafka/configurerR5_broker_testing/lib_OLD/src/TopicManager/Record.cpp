#include "Record.hpp"

Record::Record() : data("NOT VALID")
{
}

Record::Record(const std::string d) : data(d)
{
}

const std::string &Record::getData() const
{
    return data;
}

void Record::to_json(json &j) const
{
    j = json{{"data", data}};
}

void Record::from_json(const json &j)
{
    data = j.at("data").get<std::string>();
}

void to_json(nlohmann::json &j, const Record &obj)
{
    obj.to_json(j);
}
