#include "NaoLib/Robot.h"
#include <sstream>
#include <cstdlib>
#include <ctime> 
#include <iostream> 
#include <alerror/alerror.h>
#include <alproxies/almemoryproxy.h>
#include <NaoLib/Representations/RinobotMemoryMap.h>
#include <signal.h>

extern void LoadRinobotMemoryMap();

using namespace Rinobot::Nao;
using namespace std;
using namespace AL;

bool RobotModule::InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker)
{
    return InitFromRobot(broker, NULL);
}

bool RobotModule::InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker, RobotModuleSettings *settings)
{
    return false;
}

void RobotModule::DestroyFromRobot()
{
    
}

const string Robot::LOCAL = "127.0.0.1";
bool Robot::exitOnStop = false;
bool Robot::isRemote = true;
bool Robot::isSimulated = false;
bool Robot::isRunning = false;
bool Robot::inited = false;
string Robot::connection = "Unknown";
boost::shared_ptr<ALBroker>  Robot::broker = boost::shared_ptr<ALBroker>();
vector<RobotModule*> Robot::modules = vector<RobotModule*>();
int Robot::playerNumber = -1;
int Robot::teamNumber = -1;
int Robot::teamColor = TEAM_BLACK;
VoidFunc Robot::onDisconnect = NULL;
ALMemoryProxy *Robot::memory = NULL;

void Robot::RunHandler(int s)
{
    Robot::Disconnect(true);
    if(exitOnStop)
        exit(0);
}

void Robot::ExitOnStop(bool exitOnStop)
{
    Robot::exitOnStop = exitOnStop;
}

void Robot::OnDisconnect(VoidFunc fn)
{
    onDisconnect = fn;
}

void Robot::Init()
{
    if(!inited)
    {
        LoadRinobotMemoryMap();
        signal (SIGINT, Robot::RunHandler);
        inited = true;
    }
}

bool Robot::Connect(int argc, char *argv[])
{
    string ip = "127.0.0.1";
    int port = 9559;
    if(argc > 1)
        ip = string(argv[1]);
    if(argc > 2)
        port = atoi(argv[2]);
    return Connect(ip, port);
}

bool Robot::Connect(string ip, int port)
{
    Init();
    srand (time(NULL));

    stringstream ss;
    ss << "Robot" << rand();
    string brokerName = ss.str();
    try
    {
        broker = ALBroker::createBroker(brokerName, "", 0, ip, port);
        ss << " [" << ip << ":" << port << "]";
        connection = ss.str();
        if(ip.compare(LOCAL) == 0 || ip.compare("localhost") == 0)
            isRemote = false;
        else
            isRemote = true;
        if(port == 9559)
            isSimulated = false;
        else
            isSimulated = true;
        isRunning = true;
        memory = new ALMemoryProxy(broker);
        memory->insertData("Rinobot/connected", 1);
        cout << endl << "Connected to " << connection << endl;
    }
    catch (const AL::ALError& e) 
    {
        std::cerr << e.what() << std::endl;
        cout << endl << "Could  not connect to " << connection << endl;
    }
    if(isRunning)
    {
        try
        {
            teamNumber = memory->getData("GameCtrl/teamNumber");
            teamColor = memory->getData("GameCtrl/teamColour");
            playerNumber = memory->getData("GameCtrl/playerNumber");
        }
        catch (const AL::ALError& e) 
        {}
    }
}

void Robot::Disconnect(bool keep)
{
    if(onDisconnect != NULL)
    {
        onDisconnect();
    }
    bool running = isRunning;
    isRunning = false;
    if(!keep)
    {
        for(int i = 0; i < modules.size(); i++)
        {
            modules[i]->DestroyFromRobot();
            delete modules[i];
        }
        modules.clear();
    }
    if(memory != NULL)
    {
        memory->insertData("Rinobot/connected", 0);
        delete memory;
        memory = NULL;
    }
    if(running)
        cout << endl << "Disconnected from " << connection << endl;
}

bool Robot::IsConnected()
{
    return (bool)broker;
}

bool Robot::IsRemote()
{
    return isRemote;
}

bool Robot::IsSimulated()
{
    return isSimulated;
}

bool Robot::IsRunning()
{
    if(isRunning == false)
        return false;
    try
    {
        ALValue connected = memory->getData("Rinobot/connected");
        if((int)connected == 0)
        {
            Disconnect(true);
        }
    }
    catch (const AL::ALError& e) 
    {
        std::cerr << e.what() << std::endl;   
    }
    return isRunning;
}

string Robot::GetConnection()
{
    return connection;
}

void Robot::GameControl(int playerNumber, int teamNumber, int teamColor)
{
    if(!IsConnected())
        return;
    if(playerNumber != Robot::playerNumber || teamNumber != Robot::teamNumber || teamColor != Robot::teamColor)
    {
        memory->insertData("GameCtrl/teamNumber", teamNumber);
        memory->insertData("GameCtrl/teamColour", teamColor);
        memory->insertData("GameCtrl/playerNumber", playerNumber);
        Robot::teamColor = teamColor;
        Robot::teamNumber = teamNumber;
        Robot::playerNumber = playerNumber;
    }
}

int Robot::GetTeamColor()
{
    return Robot::teamColor;
}
int Robot::GetTeamNumber()
{
    return Robot::teamNumber;
}
int Robot::GetPlayerNumber()
{
    return Robot::playerNumber;
}