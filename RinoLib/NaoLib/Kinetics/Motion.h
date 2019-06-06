#ifndef _RINOBOT_NAO_LIB_KINETICS_MOTION_H_
#define _RINOBOT_NAO_LIB_KINETICS_MOTION_H_

#include <string>
#include <vector>
#include <map>
#include <alproxies/almotionproxy.h>
#include "NaoLib/Robot.h"
#include "NaoLib/Kinetics/Types.h"

namespace Rinobot
{
    namespace Nao
    {
        namespace Kinetics
        {
            typedef AL::ALValue MotionValue;

            // A classe Speech permite converter texto em voz no robô
            class Motion : public RobotModule
            {
                public:
                    enum Hands { LeftHand = 0x1, RightHand = 0x2, BothHands = 0x3};
                    enum Frames { FrameTorso = 0, FrameWorld = 1, FrameRobot = 2 };
                private:
                    AL::ALMotionProxy *proxy;
                    std::string connection;
                    std::string ip;
                    int port;
                    bool isModule;
                public:
                    Motion();
                    static Motion Ip(std::string = "127.0.0.1", int = 9559);
                    static Motion Nao();
                    static Motion Open(std::string, int = 9559);
                    
                    bool InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker);

                    virtual ~Motion();
                    bool Connect();
                    const bool IsConnected() const;

                    const std::string Connection() const;

                    void WakeUp();
                    void Rest();
                    bool IsWakeUp();

                    void SetStiffness(std::string, float);
                    void SetStiffnesses(std::vector<std::string>&, std::vector<float>&);
                    float GetStiffness(std::string);
                    std::vector<float> GetStiffnesses(std::vector<std::string>&);
                    void StiffnessInterpolation(std::vector<std::string>&, std::vector<float>&, std::vector<float>&);

                    void AnglesInterpolation(std::vector<std::string>&, std::vector<float>&, std::vector<float>&, bool = false);
                    void AnglesInterpolation(std::vector<std::string>&, std::vector<float>&, float);
                    void AnglesInterpolation(std::vector<std::string>&, std::vector<float>&, std::vector<float>&);
                    void AnglesInterpolation(MotionValue& names, MotionValue& angleLists, MotionValue& timeLists, bool isAbsolute);
                    void AnglesInterpolationWithSpeed(MotionValue& names, MotionValue& targetAngles, float maxSpeed);
                    void AnglesInterpolationBezier(MotionValue& jointNames, MotionValue& times, MotionValue& controlPoints);

                    float GetAngle(std::string, bool = true);
                    std::vector<float> GetAngles(std::vector<std::string>&, bool = true);
                    void SetAngle(std::string, float, float);
                    void SetAngles(std::vector<std::string>&, std::vector<float>&, float);
                    void ChangeAngle(std::string, float, float);
                    void ChangeAngles(std::vector<std::string>&, std::vector<float>&, float);
                    void OpenHand(Hands);
                    void CloseHand(Hands);

                    void MoveInit();
                    void WaitMove();
                    bool IsMoving();
                    void Stop();
                    void Move(const float, const float, const float);
                    void Move(const float, const float, const float, std::map<std::string, float>&);
                    void Move(const Position&);
                    void Move(const Position&, std::map<std::string, float>&);
                    void MoveToward(const float, const float, const float);
                    void MoveToward(const float, const float, const float, std::map<std::string, float>&);
                    void MoveToward(const Position&);
                    void MoveToward(const Position&, std::map<std::string, float>&);
                    void MoveTo(const float, const float, const float);
                    void MoveTo(const float, const float, const float, std::map<std::string, float>&);
                    void MoveTo(const Position&);
                    void MoveTo(const Position&, std::map<std::string, float>&);
                    void MoveTo(std::vector<Position>&);
                    void MoveTo(std::vector<Position>&, std::map<std::string, float>&);
                    void MoveTo(const float, const float, const float, const float);
                    void MoveTo(const Position&, const float);

                    void EnableBalancer(bool isEnabled);
                    void FootState(std::string stateName, std::string supportLeg);
                    void EnableBalanceConstraint(bool isEnabled, std::string supportLeg);
                    void GoToBalance(std::string supportLeg, float duration);
                    void EnableEffectorControl(std::string effectorName, bool isEnabled);
                    void SetEffectorControl(std::string effectorName, MotionValue& targetCoordinate);
                    void EnableEffectorOptimization(std::string effectorName, bool isEnabled);

                    std::vector<float> GetFramePosition(std::string name, Frames frame, bool useSensorValues);
                    void SetFramePositions(MotionValue &effectorNames, MotionValue &frame, MotionValue &position, float fractionMaxSpeed, MotionValue &axisMask);

            };
        }
    }
}

#endif