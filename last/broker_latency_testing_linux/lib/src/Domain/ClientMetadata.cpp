#include "ClientMetadata.hpp"

ClientMetadata::ClientMetadata() {}

ClientMetadata::ClientMetadata(int value) : id(value) {}

int ClientMetadata::getId() const
{
    return id;
}

void ClientMetadata::setId(int i) {
    id = i;
}
