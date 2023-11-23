#ifndef CLIENTMETADATA_H
#define CLIENTMETADATA_H

class ClientMetadata
{
private:
    int id;

public:
    ClientMetadata();
    ClientMetadata(int value);

    int getId() const;
    void setId(int id);
};


#endif
