#include "NaoLib/Communication/Communicator.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h>
#include <algorithm>

using namespace AL;
using namespace std;
using namespace Rinobot::Nao::Utils;
using namespace Rinobot::Nao::Communication;

Communicator::Communicator(const string ip, const int port, const string broadcastIp)
{
    this->ip = ip;
    this->port = port;
    this->broadcastIp = broadcastIp;
    this->callback = NULL;
    runningClient = runningServer = false;
}

bool Communicator::Start()
{   
    if(!runningServer)
    {
        // ------------------------- Server ---------------------------
        int on = 1;
        /* socket creation */
        sdServ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if( sdServ < 0 ) 
        {
            cerr << "Communicator: Cannot open server socket" << endl;
            return false;
        }

        if (setsockopt(sdServ, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) == -1) 
        {
            cerr << "Communicator: Cannot set server socket to reuse address" << endl;
            return false;
        }

        if (setsockopt(sdServ, SOL_SOCKET, SO_BROADCAST, &on, sizeof on) == -1) 
        {
            cerr << "Communicator: Cannot set server socket to broadcast" << endl;
            return false;
        }

        /* bind local server port */
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servAddr.sin_port = htons(port);
        rc = bind (sdServ, (struct sockaddr *) &servAddr,sizeof(servAddr));
        if( rc < 0 ) 
        {
            cerr << "Communicator: Cannot bind server port number " << port << endl;
            return false;
        }

        rc = pthread_create(&servThread, NULL, &ServThreadFn, (void*)this);
        if(rc != 0)
        {
            cerr << "Communicator: Failed to start server" << endl;
            return false;
        }
        runningServer = true;
    }
    if(!runningClient)
    {
        // ------------------------- Client ---------------------------
        int on = 1;
        h = gethostbyname(broadcastIp.c_str());
        if(h == NULL) 
        {
            cerr << "Communicator: unknown client host " << broadcastIp << endl;
            return false;
        }

        remoteServAddr.sin_family = h->h_addrtype;
        memcpy((char *) &remoteServAddr.sin_addr.s_addr, 
            h->h_addr_list[0], h->h_length);
        remoteServAddr.sin_port = htons(port);
        
        sdCli = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if( sdCli < 0 ) 
        {
            cerr << "Communicator: Cannot open client socket" << endl;
            return false;
        }

        if (setsockopt(sdCli, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) == -1) 
        {
            cerr << "Communicator: Cannot set client socket to reuse address" << endl;
            return false;
        }

        if (setsockopt(sdCli, SOL_SOCKET, SO_BROADCAST, &on, sizeof on) == -1) 
        {
            cerr << "Communicator: Cannot set client socket to broadcast" << endl;
            return false;
        }

        /* bind any port */
        cliAddr.sin_family = AF_INET;
        cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        cliAddr.sin_port = htons(0);

        rc = bind(sdCli, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
        if( rc <0) 
        {
            cerr << "Communicator: Cannot bind client port number " << port << endl;
            return false;
        }

        rc = pthread_create(&cliThread, NULL, &CliThreadFn, (void*)this);
        if(rc != 0)
        {
            cerr << "Communicator: Failed to start client" << endl;
            return false;
        }
        runningClient = true;
    }

    return true;
}

void Communicator::Stop()
{
    runningClient = false;
    runningServer = false;
}

void Communicator::SetMessageCallback(MessageCallback callback)
{
    this->callback = callback;
}

struct SPLStandardMessage* Communicator::Message()
{
    return &message;
}

void *Communicator::ServThreadFn(void *arg)
{
    ((Communicator*)arg)->UpdateServer();
    pthread_exit(NULL);
}

void *Communicator::CliThreadFn(void *arg)
{
    ((Communicator*)arg)->UpdateClient();
    pthread_exit(NULL);
}

void Communicator::UpdateClient()
{
    while(runningClient)
    {
        rc = sendto(sdCli, &message, sizeof(struct SPLStandardMessage), 0, (struct sockaddr *) &remoteServAddr, sizeof(remoteServAddr));
        Timer::Wait(1000);
    }
    close(sdCli);
}

void Communicator::UpdateServer()
{
    int maxMsg = sizeof(struct SPLStandardMessage);
    int n = 0;
    while (runningServer)
    {
        memset(indata, 0x0, maxMsg);
        cliLen = sizeof(remoteCliAddr);
        n = recvfrom(sdServ, indata, maxMsg, 0, (struct sockaddr *) &remoteCliAddr, (socklen_t*)&cliLen);
        if(n < 0) 
        {
            cout << "Communicator: Failed to receive data" << endl;
        }
        else
        {
            string rIp = string(inet_ntoa(remoteCliAddr.sin_addr));
            int rPort = ntohs(remoteCliAddr.sin_port);
            if(rIp.compare(ip) != 0)
            {
                if(callback != NULL)
                {
                    struct SPLStandardMessage message;
                    memcpy(&message, indata, maxMsg);
                    callback(rIp, rPort, message);
                }
            }
        }
        if(Robot::IsRunning() == false)
        {
            runningClient = runningServer = false;
        }
    }
    close(sdServ);
}