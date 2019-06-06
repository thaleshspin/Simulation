#include "NaoLib/Kinetics/Motion.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>

using namespace AL;
using namespace std;
using namespace Rinobot::Nao::Kinetics;

Motion::Motion()
{
    this->proxy = NULL;
    this->ip = "";
    this->port = -1;
    this->connection = "";
    this->isModule = false;
}

Motion Motion::Ip(string ip, int port)
{
    Motion Motion;
    Motion.ip = ip;
    Motion.port = port;
    stringstream ss;
    ss << ip << ":" << port;
    Motion.connection = ss.str();
    return Motion;
}

Motion Motion::Nao()
{
    Motion Motion = Ip("127.0.0.1", 9559);
    return Motion;
}

Motion Motion::Open(string address, int port)
{
    return Ip(address, port);
}

bool Motion::InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker)
{
    if(Robot::IsConnected())
    {
        try
        {
            proxy = new ALMotionProxy(broker);
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

Motion::~Motion()
{
    if(proxy != NULL)
    {
        delete proxy;
        proxy = NULL;
    }
}

bool Motion::Connect()
{
    bool success = false;
    if(isModule)
        return true;
    if(port > 0 && !ip.empty())
    {
        try
        {
            proxy = new ALMotionProxy(ip, port);
            success = true;
        }
        catch(const AL::ALError& e)
        {
            proxy = NULL;
        }
    }
    return success;
}

const bool Motion::IsConnected() const
{
    return (proxy != NULL);
}

const string Motion::Connection() const
{
    return connection;
}

void Motion::WakeUp()
{
    if(IsConnected())
        proxy->wakeUp();
}

void Motion::Rest()
{
    if(IsConnected())
        proxy->rest();
}

bool Motion::IsWakeUp()
{
    if(IsConnected())
        return proxy->robotIsWakeUp();
    return false;
}

void Motion::SetStiffness(string name, float value)
{
    if(IsConnected())
        proxy->setStiffnesses(name, value);
}

void Motion::SetStiffnesses(vector<string> &names, vector<float> &values)
{
    if(IsConnected())
        proxy->setStiffnesses(names, values);
}

float Motion::GetStiffness(string name)
{
    if(IsConnected())
        return (proxy->getStiffnesses(name))[0];
    return 0;
}

vector<float> Motion::GetStiffnesses(vector<string> &names)
{
    if(IsConnected())
        return proxy->getStiffnesses(names);
    return vector<float>();
}

void Motion::StiffnessInterpolation(vector<string> &names, vector<float> &values, vector<float> &times)
{
    if(IsConnected())
        return proxy->stiffnessInterpolation(names, values, times);
}

void Motion::AnglesInterpolation(vector<string> &names, vector<float> &values, vector<float> &times, bool isAbsolute)
{
    if(IsConnected())
        return proxy->angleInterpolation(names, values, times, isAbsolute);
}

void Motion::AnglesInterpolation(vector<string> &names, vector<float> &values, float maxSpeedFraction)
{
    if(IsConnected())
        return proxy->angleInterpolationWithSpeed(names, values, maxSpeedFraction);
}

void Motion::AnglesInterpolation(vector<string> &names, vector<float> &times, vector<float> &values)
{
    if(IsConnected())
        return proxy->angleInterpolationBezier(names, times, values);
}

void Motion::AnglesInterpolation(MotionValue& names, MotionValue& angleLists, MotionValue& timeLists, bool isAbsolute)
{
    if(IsConnected())
        return proxy->angleInterpolation(names, angleLists, timeLists, isAbsolute);
}

void Motion::AnglesInterpolationWithSpeed(MotionValue& names, MotionValue& targetAngles, float maxSpeed)
{
    if(IsConnected())
        return proxy->angleInterpolationWithSpeed(names, targetAngles, maxSpeed);
}

void Motion::AnglesInterpolationBezier(MotionValue& jointNames, MotionValue& times, MotionValue& controlPoints)
{
    if(IsConnected())
        return proxy->angleInterpolationWithSpeed(jointNames, times, controlPoints);
}

float Motion::GetAngle(string name, bool useSensors)
{
    if(IsConnected())
        return (proxy->getAngles(name, useSensors))[0];
    return 0;
}

vector<float> Motion::GetAngles(vector<string> &names, bool useSensors)
{
    if(IsConnected())
        return proxy->getAngles(names, useSensors);
    return vector<float>();
}

void Motion::SetAngle(string name, float value, float maxSpeedFraction)
{
    if(IsConnected())
        proxy->setAngles(name, value, maxSpeedFraction);
}

void Motion::SetAngles(vector<string> &names, vector<float> &values, float maxSpeedFraction)
{
    if(IsConnected())
        proxy->setAngles(names, values, maxSpeedFraction);
}

void Motion::ChangeAngle(string name, float value, float maxSpeedFraction)
{
    if(IsConnected())
        proxy->changeAngles(name, value, maxSpeedFraction);
}

void Motion::ChangeAngles(vector<string> &names, vector<float> &values, float maxSpeedFraction)
{
    if(IsConnected())
        proxy->changeAngles(names, values, maxSpeedFraction);
}


void Motion::OpenHand(Hands hands)
{
    if(!IsConnected())
        return;
    if(hands & LeftHand)
        proxy->openHand("LHand");
    if(hands & RightHand)
        proxy->openHand("RHand");
}

void Motion::CloseHand(Hands hands)
{
    if(!IsConnected())
        return;
    if(hands & LeftHand)
        proxy->closeHand("LHand");
    if(hands & RightHand)
        proxy->closeHand("RHand");
}

void Motion::MoveInit()
{
    if(!IsConnected())
        return;
    proxy->moveInit();
}

void Motion::WaitMove()
{
    if(!IsConnected())
        return;
    proxy->waitUntilMoveIsFinished();
}

bool Motion::IsMoving()
{
    if(!IsConnected())
        return false;
    return proxy->moveIsActive();
}

void Motion::Stop()
{
    if(!IsConnected())
        return;
    proxy->stopMove();
}

void Motion::Move(const float x, const float y, const float theta)
{
    if(!IsConnected())
        return;
    proxy->move(x, y, theta);
}

void Motion::Move(const float x, const float y, const float theta, map<string, float> &config)
{
    if(!IsConnected())
        return;
    ALValue _config;
    _config.arraySetSize(config.size());
    int i = 0;
    for(map<string,float>::iterator it = config.begin(); it != config.end(); it++)
    {
        _config[i++] = ALValue::array(it->first, it->second);
    }
    proxy->move(x, y, theta, _config);
}

void Motion::Move(const Position &position)
{
    Move(position.x, position.y, position.theta);
}

void Motion::Move(const Position &position, map<string, float> &config)
{
    Move(position.x, position.y, position.theta, config);
}

void Motion::MoveToward(const float x, const float y, const float theta)
{
    if(!IsConnected())
        return;
    proxy->moveToward(x, y, theta);
}

void Motion::MoveToward(const float x, const float y, const float theta, map<string, float> &config)
{
    if(!IsConnected())
        return;
    ALValue _config;
    _config.arraySetSize(config.size());
    int i = 0;
    for(map<string,float>::iterator it = config.begin(); it != config.end(); it++)
    {
        _config[i++] = ALValue::array(it->first, it->second);
    }
    proxy->moveToward(x, y, theta, _config);
}

void Motion::MoveToward(const Position &position)
{
    MoveToward(position.x, position.y, position.theta);
}

void Motion::MoveToward(const Position &position, map<string, float> &config)
{
    MoveToward(position.x, position.y, position.theta, config);
}

void Motion::MoveTo(const float x, const float y, const float theta)
{
    if(!IsConnected())
        return;
    proxy->moveTo(x, y, theta);
}

void Motion::MoveTo(const float x, const float y, const float theta, map<string, float> &config)
{
    if(!IsConnected())
        return;
    ALValue _config;
    _config.arraySetSize(config.size());
    int i = 0;
    for(map<string,float>::iterator it = config.begin(); it != config.end(); it++)
    {
        _config[i++] = ALValue::array(it->first, it->second);
    }
    proxy->moveTo(x, y, theta, _config);
}

void Motion::MoveTo(const Position &position)
{
    MoveTo(position.x, position.y, position.theta);
}

void Motion::MoveTo(const Position &position, map<string, float> &config)
{
    MoveTo(position.x, position.y, position.theta, config);
}

void Motion::MoveTo(vector<Position> &positions)
{
    if(!IsConnected())
        return;
    ALValue _positions;
    _positions.arraySetSize(positions.size());
    int i = 0;
    for(vector<Position>::iterator it = positions.begin(); it != positions.end(); it++)
    {
        _positions[i++] = ALValue::array(it->x, it->y, it->theta);
    }
    proxy->moveTo(_positions);
}

void Motion::MoveTo(vector<Position> &positions, map<string, float> &config)
{
    if(!IsConnected())
        return;
    ALValue _positions;
    _positions.arraySetSize(positions.size());
    int i = 0;
    for(vector<Position>::iterator it = positions.begin(); it != positions.end(); it++)
    {
        _positions[i++] = ALValue::array(it->x, it->y, it->theta);
    }
    ALValue _config;
    _config.arraySetSize(config.size());
    i = 0;
    for(map<string,float>::iterator it = config.begin(); it != config.end(); it++)
    {
        _config[i++] = ALValue::array(it->first, it->second);
    }
    proxy->moveTo(_positions, _config);
}

void Motion::MoveTo(const float x, const float y, const float theta, const float time)
{
    if(!IsConnected())
        return;
    proxy->moveTo(x, y, theta, time);
}

void Motion::MoveTo(const Position &position, const float time)
{
    MoveTo(position.x, position.y, position.theta, time);
}

void Motion::EnableBalancer(bool isEnabled)
{
    if(!IsConnected())
        return;
    proxy->wbEnable(isEnabled);
}

void Motion::FootState(std::string stateName, std::string supportLeg)
{
    if(!IsConnected())
        return;
    proxy->wbFootState(stateName, supportLeg);
}

void Motion::EnableBalanceConstraint(bool isEnabled, std::string supportLeg)
{
    if(!IsConnected())
        return;
    proxy->wbEnableBalanceConstraint(isEnabled, supportLeg);
}

void Motion::GoToBalance(std::string supportLeg, float duration)
{
    if(!IsConnected())
        return;
    proxy->wbGoToBalance(supportLeg, duration);
}

void Motion::EnableEffectorControl(std::string effectorName, bool isEnabled)
{
    if(!IsConnected())
        return;
    proxy->wbEnableEffectorControl(effectorName, isEnabled);
}

void Motion::SetEffectorControl(std::string effectorName, MotionValue& targetCoordinate)
{
    if(!IsConnected())
        return;
    proxy->wbSetEffectorControl(effectorName, targetCoordinate);
}

void Motion::EnableEffectorOptimization(std::string effectorName, bool isEnabled)
{
    if(!IsConnected())
        return;
    proxy->wbEnableEffectorOptimization(effectorName, isEnabled);
}


std::vector<float> Motion::GetFramePosition(std::string name, Frames frame, bool useSensorValues)
{
    if(!IsConnected())
        return vector<float>();
    return proxy->getPosition(name, (int)frame, useSensorValues);
}

void Motion::SetFramePositions(MotionValue &effectorNames, MotionValue &frame, MotionValue &position, float fractionMaxSpeed, MotionValue &axisMask)
{
    if(!IsConnected())
        return;
    proxy->setPositions(effectorNames, frame, position, fractionMaxSpeed, axisMask);
}