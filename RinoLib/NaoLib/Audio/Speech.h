#ifndef _RINOBOT_NAO_LIB_AUDIO_SPEECH_H_
#define _RINOBOT_NAO_LIB_AUDIO_SPEECH_H_

#include <string>
#include <alproxies/altexttospeechproxy.h>
#include <alproxies/alanimatedspeechproxy.h>
#include "NaoLib/Robot.h"

namespace Rinobot
{
    namespace Nao
    {
        namespace Audio
        {
            // A classe Speech permite converter texto em voz no robô
            class Speech : public RobotModule
            {
                public:
                    enum BodyLanguage { BodyLanguageDisabled = 0x0, BodyLanguageRandom = 0x1, BodyLanguageContextual = 0x2 };
                private:
                    AL::ALTextToSpeechProxy *proxy;
                    AL::ALAnimatedSpeechProxy *animatedProxy;
                    std::string connection;
                    std::string ip;
                    int port;
                    bool isModule, isAnimated;
                public:
                    Speech();
                    static Speech Ip(std::string = "127.0.0.1", int = 9559);
                    static Speech Nao();
                    static Speech Open(std::string, int = 9559);
                    
                    bool InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker);

                    virtual ~Speech();
                    bool Connect();
                    const bool IsConnected() const;

                    const std::string Connection() const;

                    void SetAnimated(bool);
                    bool IsAnimated();

                    void Say(const std::string&);      
                    void Say(const std::string&, const std::string&);
                    void Say(const std::string&, const BodyLanguage&);
                    void SetLanguage(const std::string&);
                    void SetBodyLanguage(const BodyLanguage&);

            };
        }
    }
}

#endif