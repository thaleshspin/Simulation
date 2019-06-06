#ifndef _RINOBOT_NAO_LIB_KINETICS_POSTURE_H_
#define _RINOBOT_NAO_LIB_KINETICS_POSTURE_H_

#include <string>
#include <vector>
#include <alproxies/alrobotpostureproxy.h>
#include "NaoLib/Robot.h"

namespace Rinobot
{
    namespace Nao
    {
        namespace Kinetics
        {
            // A classe Speech permite converter texto em voz no robô
            class Posture : public RobotModule
            {
                private:
                    AL::ALRobotPostureProxy *proxy;
                    std::string connection;
                    std::string ip;
                    int port;
                    bool isModule;
                public:
                    Posture();
                    static Posture Ip(std::string = "127.0.0.1", int = 9559);
                    static Posture Nao();
                    static Posture Open(std::string, int = 9559);
                    
                    bool InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker);

                    virtual ~Posture();
                    bool Connect();
                    const bool IsConnected() const;

                    const std::string Connection() const;

                    std::vector<std::string> GetPostureList();
                    std::string GetPosture();
                    bool GoToPosture(const std::string&, const float);
                    bool ApplyPosture(const std::string&, const float);
                    std::string GetPostureFamily();
                    std::vector<std::string> GetPostureFamilyList();
                    void Stop();
                    void SetMaxTries(const int);
            };
        }
    }
}

#endif