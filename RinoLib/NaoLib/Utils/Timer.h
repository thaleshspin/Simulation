#ifndef _RINOBOT_NAO_LIB_UTIL_TIMER_H_
#define _RINOBOT_NAO_LIB_UTIL_TIMER_H_

#include <time.h>

namespace Rinobot
{
    namespace Nao
    {
        namespace Utils
        {
            // A classe Timer permite analisar o tempo de execução de um contexto
            class Timer
            {
                private:
                    struct timespec start, end;
                public:
                    Timer();
                    void Init();

                    double Micros();
                    double Millis();
                    double Seconds();


                    static void Wait(int ms = 100);
            };
        }
    }
}

#endif