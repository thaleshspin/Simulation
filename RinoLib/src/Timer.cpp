#include "NaoLib/Utils/Timer.h"
#include <unistd.h>

using namespace Rinobot::Nao::Utils;
using namespace std;

Timer::Timer()
{
    Init();
}

void Timer::Init()
{
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
}

double Timer::Micros()
{
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double value = ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000);
    Init();
    return value;
}

double Timer::Millis()
{
    return Micros() / 1000.0;
}

double Timer::Seconds()
{
    return Millis() / 1000.0;
}

void Timer::Wait(int ms)
{
    usleep( 1000 * ms );
}