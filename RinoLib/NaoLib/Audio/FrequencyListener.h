#ifndef _RINOBOT_NAO_LIB_AUDIO_FREQUENCY_LISTENER_H_
#define _RINOBOT_NAO_LIB_AUDIO_FREQUENCY_LISTENER_H_

#include "NaoLib/Robot.h"
#include "NaoLib/Audio/Listener.h"
#include "NaoLib/Tools/wavelib/header/wavelib.h"

namespace Rinobot
{
    namespace Nao
    {
        namespace Audio
        {
            typedef void (*FrequencyCallback)(void*, double); //Função a ser chamada quando a frequencia é detectada
            class FrequencyListener : public Listener
            {
                private:
                    //Objetos para executar a DWT
                    void* arg;
                    wave_object obj;
                    wt_object wt;
                    FrequencyCallback callback;
                    double duration; //Armazena a duração
                    double thresh;   //Representa o limite entre frequencia e não frequencia
                    bool detecting;  //Indica se está no meio de uma detecção
                public:
                    FrequencyListener();
                    virtual ~FrequencyListener();

                    void SetFrequency(int low, int high);
                    void SetCallback(FrequencyCallback callback, void* arg);
                    void SetThreshold(double thresh);
                    virtual void process(const int & nbOfChannels, const int & nbrOfSamplesByChannel, const AL_SOUND_FORMAT * buffer, const AL::ALValue & timeStamp);
            };
        }
    }
}

#endif