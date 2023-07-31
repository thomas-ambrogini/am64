#include "CommunicationFactory.hpp"
#include "StandardOutputLogger.hpp"
#include "RPMessageEndpoint.hpp"
#include "EndpointFactory.hpp"

#include <chrono>

#define gMsgEchoCount (100000u)

void fillBuffer(char *buffer, int bufferSize)
{
	memset(buffer, 'c', bufferSize - 1);
	buffer[bufferSize - 1] = '\0';
}

int main(int argc, char *argv[])
{
	uint32_t msg;
	uint16_t msgSize;
	char packet_buf[512], packet_buf_read[512], packet_buf_read2[512] = {0};

	StandardOutputLogger logger;
	RPMessageEndpoint *sourceEndpoint = static_cast<RPMessageEndpoint *>(EndpointFactory::createEndpoint(CommunicationType::RPMessageLinux));
	sourceEndpoint->setCoreId(1);
	sourceEndpoint->setServiceEndpoint(14);
	Communication *communication = CommunicationFactory::createCommunication(CommunicationType::RPMessageLinux, *sourceEndpoint, logger);

	RPMessageEndpoint *destinationEndpoint = static_cast<RPMessageEndpoint *>(EndpointFactory::createEndpoint(CommunicationType::RPMessageLinux));
	destinationEndpoint->setCoreId(2);
	destinationEndpoint->setServiceEndpoint(14);

	fillBuffer(packet_buf, 256 - 16);
	msgSize = strlen(packet_buf) + 1;

	logger.log("I want to send the message: %s, with size: %d", packet_buf, msgSize);

	auto start = std::chrono::high_resolution_clock::now();

	for (msg = 0; msg < gMsgEchoCount; msg++)
	{
		communication->write(packet_buf, msgSize, *destinationEndpoint);
		communication->read(packet_buf_read, sizeof(packet_buf), *sourceEndpoint);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	std::cout << "Time taken: " << duration / (gMsgEchoCount) << " microseconds" << std::endl;

	delete communication;

	return 0;
}
