#include "NaoLib/Vision/VisualCompass.h"
#include <alvision/alvisiondefinitions.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <opencv2/opencv.hpp>

using namespace AL;
using namespace std;
using namespace Rinobot::Nao::Vision;

VisualCompass::VisualCompass()
{
    this->compassProxy = NULL;
    this->memoryProxy = NULL;
    this->ip = "";
    this->port = -1;
    this->connection = "";
    this->isModule = false;
    this->inited = false;
}

VisualCompass VisualCompass::Ip(string ip, int port)
{
    VisualCompass visualCompass;
    visualCompass.ip = ip;
    visualCompass.port = port;
    stringstream ss;
    ss << ip << ":" << port;
    visualCompass.connection = ss.str();
    return visualCompass;
}

VisualCompass VisualCompass::Nao()
{
    VisualCompass visualCompass = Ip("127.0.0.1", 9559);
    return visualCompass;
}

VisualCompass VisualCompass::Open(string address, int port)
{
    return Ip(address, port);
}

bool VisualCompass::InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker)
{
    if(Robot::IsConnected())
    {
        if(!Robot::IsSimulated())
        {
            try
            {
                compassProxy = new ALVisualCompassProxy(broker);
                memoryProxy = new ALMemoryProxy(broker);
                connection = Robot::GetConnection();

                compassProxy->enableReferenceRefresh(true);
                compassProxy->setResolution(kQVGA);
                compassProxy->setActiveCamera(kBottomCamera);
                clientName = "VisualCompass";
                compassProxy->subscribe(clientName);

                this->inited = true;
                this->isModule = true;
                return true;
            }
            catch(const AL::ALError& e)
            {
                cout << e.what() << endl;
                compassProxy = NULL;
                memoryProxy = NULL;
            }
        }
        else
        {
            compassProxy = NULL;
            memoryProxy = NULL;
            this->inited = true;
            this->isModule = true;
            return true;
        }
    }
    return false;
}

VisualCompass::~VisualCompass()
{
    if(compassProxy != NULL)
    {
        compassProxy->unsubscribe(clientName);
        delete compassProxy;
        compassProxy = NULL;
    }
    if(memoryProxy != NULL)
    {
        delete memoryProxy;
        memoryProxy = NULL;
    }
}

bool VisualCompass::Connect()
{
    bool success = false;
    if(isModule)
        return true;
    if(Robot::IsSimulated())
    {
        this->inited = true;
        return true;
    }
    if(port > 0 && !ip.empty())
    {
        try
        {
            compassProxy = new ALVisualCompassProxy(ip, port);
            memoryProxy = new ALMemoryProxy(ip, port);
            connection = Robot::GetConnection();

            compassProxy->enableReferenceRefresh(true);
            compassProxy->setResolution(kQVGA);
            compassProxy->setActiveCamera(kBottomCamera);
            clientName = "VisualCompass";
            compassProxy->subscribe(clientName);
            success = true;
        }
        catch(const AL::ALError& e)
        {
            cout << e.what() << endl;
            compassProxy = NULL;
            memoryProxy = NULL;
        }
    }
    return success;
}

const bool VisualCompass::IsConnected() const
{
    if(Robot::IsSimulated())
        return inited;
    return (compassProxy != NULL);
}

const string VisualCompass::Connection() const
{
    return connection;
}

bool VisualCompass::GetDeviation(float *wz, float *wy)
{
    if(Robot::IsSimulated())
    {
        return inited;
    }
    if(!IsConnected())
        return false;
    ALValue deviation;
    try 
    {
        deviation = memoryProxy->getData("VisualCompass/Deviation");
    }
    catch (const ALError& e) 
    {
        cout << e.what() << endl;
        return false;
    }

    float _wy = deviation[0][0], _wz = deviation[0][1];
    // Convert it to degrees.
    _wz = _wz * 180.0f / 3.14f;
    _wy = _wy * 180.0f / 3.14f;

    *wz = _wz;
    *wy = _wy;
    return true;
}