#ifndef _RINOBOT_NAO_LIB_AUDIO_LISTENER_H_
#define _RINOBOT_NAO_LIB_AUDIO_LISTENER_H_

#include <string>
#include <alproxies/almemoryproxy.h>
#include <alaudio/alsoundextractor.h>
#include "NaoLib/Robot.h"

namespace Rinobot
{
    namespace Nao
    {
        namespace Audio
        {
            // A classe Listener permite processar o áudio dos microfones do robô em tempo real
            class Listener : public RobotModule
            {
                private:
                    class ListenerProxy : public AL::ALSoundExtractor
                    {
                        private:
                            Listener *plistener;
                        public:
                            ListenerProxy(boost::shared_ptr<AL::ALBroker> pBroker, std::string pName, Listener *listener);
                            virtual ~ListenerProxy();
                            void init();
                            void process(const int & nbOfChannels, const int & nbrOfSamplesByChannel, const AL_SOUND_FORMAT * buffer, const AL::ALValue & timeStamp);
                    };

                    ListenerProxy *proxy;
                    std::string connection;
                    bool isModule;
                public:
                    Listener();
                    
                    virtual bool InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker);

                    virtual ~Listener();
                    bool Connect();
                    const bool IsConnected() const;

                    const std::string Connection() const;

                    virtual void process(const int & nbOfChannels, const int & nbrOfSamplesByChannel, const AL_SOUND_FORMAT * buffer, const AL::ALValue & timeStamp);

                    
            };
        }
    }
}

#endif