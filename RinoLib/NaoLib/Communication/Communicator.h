#ifndef _RINOBOT_NAO_LIB_COMMUNICATION_COMMUNICATOR_H_
#define _RINOBOT_NAO_LIB_COMMUNICATION_COMMUNICATOR_H_

#include <string>
#include <vector>
#include <map>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "NaoLib/Robot.h"
#include "NaoLib/Utils/Timer.h"
#include "NaoLib/GameControl/SPLStandardMessage.h"

namespace Rinobot
{
    namespace Nao
    {
        namespace Communication
        {
            typedef void (*MessageCallback)(std::string, int, struct SPLStandardMessage);
            class Communicator
            {
                private:
                    int sdServ, sdCli, rc, cliLen;
                    struct sockaddr_in remoteCliAddr, cliAddr, remoteServAddr, servAddr;
                    struct hostent *h;
                    std::string ip;
                    std::string broadcastIp;
                    int port;
                    MessageCallback callback;
                    struct SPLStandardMessage message;
                    unsigned char indata[sizeof(struct SPLStandardMessage)];
                    bool runningServer, runningClient;
                    pthread_t servThread, cliThread;
                    static void *ServThreadFn(void*);
                    static void *CliThreadFn(void*);
                    void UpdateServer();
                    void UpdateClient();
                public:
                    Communicator(const std::string ip, const int port, const std::string broadcastIp = "255.255.255.255");
                    bool Start();
                    void Stop();

                    void SetMessageCallback(MessageCallback callback);
                    struct SPLStandardMessage* Message();
            };
        }
    }
}

#endif