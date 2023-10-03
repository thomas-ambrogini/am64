#include "UDPEndpoint.hpp"

UDPEndpoint::UDPEndpoint() : ipAddress("127.0.0.1"), port(0)
{
}

UDPEndpoint::UDPEndpoint(unsigned int p) : ipAddress("127.0.0.1"), port(p)
{
}

UDPEndpoint::UDPEndpoint(const char *ip, unsigned int p) : port(p)
{
    std::strncpy(ipAddress, ip, sizeof(ipAddress));
    ipAddress[sizeof(ipAddress) - 1] = '\0';
}

UDPEndpoint::~UDPEndpoint()
{
}

void UDPEndpoint::printEndpointInformation(const Logger &logger) const
{
    logger.log("[UDPSocket] IPAddress: %s, Port: %d", ipAddress, port);
}

void UDPEndpoint::to_json(json &j) const
{
    j = json{{"type", "UDPSocket"}, {"ip", ipAddress}, {"port", port}};
}

void UDPEndpoint::from_json(const json &j)
{
    setIpAddress(j.at("ip").get<std::string>().c_str());
    port = j.at("port").get<int>();
}

void UDPEndpoint::setIpAddress(const char *ip)
{
    std::strncpy(ipAddress, ip, sizeof(ipAddress));
    ipAddress[sizeof(ipAddress) - 1] = '\0';
}

void UDPEndpoint::setPort(unsigned int p)
{
    port = p;
}

unsigned int UDPEndpoint::getPort() const
{
    return port;
}

const char *UDPEndpoint::getIpAddress() const
{
    return ipAddress;
}