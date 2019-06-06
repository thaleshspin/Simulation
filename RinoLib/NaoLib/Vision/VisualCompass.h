#ifndef _RINOBOT_NAO_LIB_KINETICS_POSTURE_H_
#define _RINOBOT_NAO_LIB_KINETICS_POSTURE_H_

#include <string>
#include <vector>
#include <alproxies/alvisualcompassproxy.h>
#include <alproxies/almemoryproxy.h>
#include "NaoLib/Robot.h"

namespace Rinobot
{
    namespace Nao
    {
        namespace Vision
        {
            class VisualCompass : public RobotModule
            {
                private:
                    AL::ALVisualCompassProxy *compassProxy;
                    AL::ALMemoryProxy *memoryProxy;
                    std::string clientName;
                    std::string connection;
                    std::string ip;
                    int port;
                    bool isModule;
                    bool inited;
                public:
                    VisualCompass();
                    static VisualCompass Ip(std::string = "127.0.0.1", int = 9559);
                    static VisualCompass Nao();
                    static VisualCompass Open(std::string, int = 9559);
                    
                    bool InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker);

                    virtual ~VisualCompass();
                    bool Connect();
                    const bool IsConnected() const;

                    const std::string Connection() const;

                    bool GetDeviation(float *wz, float *wy);
                    
            };
        }
    }
}

#endif