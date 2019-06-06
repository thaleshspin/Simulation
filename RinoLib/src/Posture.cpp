#include "NaoLib/Kinetics/Posture.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>

using namespace AL;
using namespace std;
using namespace Rinobot::Nao::Kinetics;

Posture::Posture()
{
    this->proxy = NULL;
    this->ip = "";
    this->port = -1;
    this->connection = "";
    this->isModule = false;
}

Posture Posture::Ip(string ip, int port)
{
    Posture Posture;
    Posture.ip = ip;
    Posture.port = port;
    stringstream ss;
    ss << ip << ":" << port;
    Posture.connection = ss.str();
    return Posture;
}

Posture Posture::Nao()
{
    Posture Posture = Ip("127.0.0.1", 9559);
    return Posture;
}

Posture Posture::Open(string address, int port)
{
    return Ip(address, port);
}

bool Posture::InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker)
{
    if(Robot::IsConnected())
    {
        try
        {
            proxy = new ALRobotPostureProxy(broker);
            connection = Robot::GetConnection();
            this->isModule = true;
            return true;
        }
        catch(const AL::ALError& e)
        {
            cout << e.what() << endl;
            proxy = NULL;
        }
    }
    return false;
}

Posture::~Posture()
{
    if(proxy != NULL)
    {
        delete proxy;
        proxy = NULL;
    }
}

bool Posture::Connect()
{
    bool success = false;
    if(isModule)
        return true;
    if(port > 0 && !ip.empty())
    {
        try
        {
            proxy = new ALRobotPostureProxy(ip, port);
            success = true;
        }
        catch(const AL::ALError& e)
        {
            proxy = NULL;
        }
    }
    return success;
}

const bool Posture::IsConnected() const
{
    return (proxy != NULL);
}

const string Posture::Connection() const
{
    return connection;
}

vector<string> Posture::GetPostureList()
{
    if(!IsConnected())
        return vector<string>();
    return proxy->getPostureList(); 
}

string Posture::GetPosture()
{
    if(!IsConnected())
        return "";
    return proxy->getPosture();
}

bool Posture::GoToPosture(const string &name, const float speed)
{
    if(!IsConnected())
        return false;
    proxy->goToPosture(name, speed);
}

bool Posture::ApplyPosture(const string &name, const float speed)
{
    if(!IsConnected())
        return false;
    proxy->applyPosture(name, speed);
}

string Posture::GetPostureFamily()
{
    if(!IsConnected())
        return "";
    return proxy->getPostureFamily();
}

vector<string> Posture::GetPostureFamilyList()
{
    if(!IsConnected())
        return vector<string>();
    return proxy->getPostureFamilyList(); 
}

void Posture::Stop()
{
    if(!IsConnected())
        return;
    return proxy->stopMove();
}

void Posture::SetMaxTries(const int maxTries)
{
    if(!IsConnected())
        return;
    return proxy->setMaxTryNumber(maxTries);
}