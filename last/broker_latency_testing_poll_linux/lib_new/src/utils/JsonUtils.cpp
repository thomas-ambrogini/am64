#include "JsonUtils.hpp"

#ifdef __unix__

json JsonUtils::readJsonFile(const std::string &filename, const Logger &logger)
{

    std::ifstream file(filename);
    if (!file.is_open())
    {
        logger.logError("Failed to open the config file");
        return json();
    }

    json jsonData;
    try
    {
        file >> jsonData;
    }
    catch (const json::exception &e)
    {
        logger.logError("Failed to parse JSON file");
        return json();
    }

    file.close();

    return jsonData;
}
#endif

#ifdef __unix__
void JsonUtils::writeJsonFile(const std::string &filename, const json jsonData, const Logger &logger)
{

    std::ofstream file(filename);

    if (!file.is_open())
    {
        logger.logError("Failed to open the config file");
    }

    std::string jsonString = jsonData.dump();

    file << jsonString;

    file.close();
}
#endif
