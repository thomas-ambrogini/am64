#include "SystemManager.hpp"


Communication * CommunicationFactory::open(CommunicationType commType, char* destination)
{
    if (commType == RPMessage) {
        return new IPC_Communication(destination);
    }
    else
        return nullptr;
}



// class SystemManager {
//     private:
//         Communication* comm;
//     public:
//         SystemManager() {
//             comm = nullptr;
//         }

//         void openConnection(CommunicationType commType) {
//             CommunicationFactory * cf = new CommunicationFactory();
//             comm = cf->open(commType);

//             delete cf;
//         }

//         ~SystemManager() {
//             if(comm) {
//                 delete comm;
//                 comm = NULL;
//             }
//         }

// }