#ifndef LATENCYDIAGNOSTIC_H
#define LATENCYDIAGNOSTIC_H

#include "Diagnostic.hpp"
#include "JsonUtils.hpp"

#include <vector>

class LatencyDiagnostic
{
public:
    LatencyDiagnostic(const CommunicationType communicationType, const Logger &logger);
    LatencyDiagnostic(const CommunicationType communicationType, const Logger &logger, const std::string configFile);
    LatencyDiagnostic(const CommunicationType communicationType, const Logger &logger, BrokerMetadata bootstrapBroker);

    void start();

private:
    ClusterMetadata cluster;
    const CommunicationType communicationType;
    const Logger &logger;

    const std::string configFile;

    void createTopic();
};

#endif
