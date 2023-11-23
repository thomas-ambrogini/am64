#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include "ConsumerMetadata.hpp"
#include "TopicMetadata.hpp"
#include "TopicFactory.hpp"
#include "Topic.hpp"
#include <vector>

class Diagnostic
{
public:
    // Diagnostic(const CommunicationType communicationType, const Logger &logger);
    // Diagnostic(const CommunicationType communicationType, const Logger &logger, BrokerMetadata bootstrapBroker);

    virtual void createBrokers() = 0;

private:
};

#endif
