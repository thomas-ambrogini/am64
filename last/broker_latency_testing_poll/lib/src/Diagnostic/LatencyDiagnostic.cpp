#include "LatencyDiagnostic.hpp"

LatencyDiagnostic::LatencyDiagnostic(const CommunicationType commType, const Logger &logger, const std::string configFile) : communicationType(commType), logger(logger), configFile(configFile)
{
    createTopic();
}

LatencyDiagnostic::LatencyDiagnostic(const CommunicationType commType, const Logger &logger) : LatencyDiagnostic(commType, logger, "../conf/configFile.json")
{
}

void LatencyDiagnostic::createTopic()
{
#ifdef __unix__

    std::string topicName = "LatencyDiagnostic";
    TopicMetadata latencyTopic(topicName);

    json jsonData = JsonUtils::readJsonFile(configFile, logger);
    ClusterMetadata startingCluster;
    startingCluster.from_json(jsonData);

    for (BrokerMetadata &broker : startingCluster.getBrokersMetadata())
    {
        broker.addTopicMetadata(latencyTopic);
        cluster.addBrokerMetadata(broker);
    }

    cluster.to_json(jsonData);
    logger.log("New cluster data: %s", jsonData.dump().c_str());

    JsonUtils::writeJsonFile(configFile, jsonData, logger);

#endif
}

void LatencyDiagnostic::start()
{
}
