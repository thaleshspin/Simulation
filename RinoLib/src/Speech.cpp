#include "NaoLib/Audio/Speech.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>

using namespace AL;
using namespace std;
using namespace Rinobot::Nao::Audio;

Speech::Speech()
{
    this->proxy = NULL;
    this->ip = "";
    this->port = -1;
    this->connection = "";
    this->isModule = false;
    this->isAnimated = false;
}

Speech Speech::Ip(string ip, int port)
{
    Speech speech;
    speech.ip = ip;
    speech.port = port;
    stringstream ss;
    ss << ip << ":" << port;
    speech.connection = ss.str();
    return speech;
}

Speech Speech::Nao()
{
    Speech speech = Ip("127.0.0.1", 9559);
    return speech;
}

Speech Speech::Open(string address, int port)
{
    return Ip(address, port);
}

bool Speech::InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker)
{
    if(Robot::IsConnected())
    {
        try
        {
            proxy = new ALTextToSpeechProxy(broker);
            animatedProxy = new ALAnimatedSpeechProxy(broker);
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

Speech::~Speech()
{
    if(proxy != NULL)
    {
        delete proxy;
        proxy = NULL;
    }
    if(animatedProxy != NULL)
    {
        delete animatedProxy;
        animatedProxy = NULL;
    }
}

bool Speech::Connect()
{
    bool success = false;
    if(isModule)
        return true;
    if(port > 0 && !ip.empty())
    {
        try
        {
            proxy = new ALTextToSpeechProxy(ip, port);
            animatedProxy = new ALAnimatedSpeechProxy(ip, port);
            success = true;
        }
        catch(const AL::ALError& e)
        {
            proxy = NULL;
        }
    }
    return success;
}

const bool Speech::IsConnected() const
{
    return (proxy != NULL && animatedProxy != NULL);
}

const string Speech::Connection() const
{
    return connection;
}

void Speech::SetAnimated(bool animated)
{
    isAnimated = animated;
}

void Speech::Say(const std::string &text)
{
    if(!IsConnected())
        return;
    if(isAnimated)
        animatedProxy->say(text);
    else
        proxy->say(text);
}

void Speech::Say(const std::string &text, const std::string &language)
{
    if(!IsConnected())
        return;
    proxy->say(text, language);
}

void Speech::Say(const std::string &text, const BodyLanguage &bodyLanguage)
{
    if(!IsConnected())
        return;
    animatedProxy->say(text);
}


void Speech::SetLanguage(const std::string &language)
{
    if(!IsConnected())
        return;
    proxy->setLanguage(language);
}

void Speech::SetBodyLanguage(const BodyLanguage &bodyLanguage)
{
    if(!IsConnected())
        return; 
    animatedProxy->setBodyLanguageMode((unsigned int)bodyLanguage);
}