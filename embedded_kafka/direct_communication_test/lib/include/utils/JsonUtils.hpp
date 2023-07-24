#ifndef JSONUTILS_H
#define JSONUTILS_H

#ifdef __unix__
#include <fstream>
#endif

#include "Logger.hpp"
#include "json.hpp"

using json = nlohmann::json;

class JsonUtils
{
public:
#ifdef __unix__
    static json readJsonFile(const std::string &filename, const Logger &logger);
    static void writeJsonFile(const std::string &filename, const json jsonData, const Logger &logger);
#endif
};

#endif
