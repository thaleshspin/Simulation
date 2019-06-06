#include "NaoLib/Audio/FrequencyListener.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>

using namespace AL;
using namespace std;
using namespace Rinobot::Nao::Audio;

FrequencyListener::FrequencyListener()
{
    char *name = "db4";    //Inicializa a biblioteca DWT
    obj = wave_init(name); // Initialize the wavelet
    wt = NULL;
    callback = NULL;
    detecting = false;
    duration = 0;
    thresh = 2.5;
}

FrequencyListener::~FrequencyListener()
{
    wave_free(obj);
    if (wt != NULL)
        wt_free(wt);
}

void FrequencyListener::SetFrequency(int lower, int high)
{
    int J = 1;     //Quantidade de niveis de decomposição da DWT
    int F = 24000; //Frequencia inicial
    int f = 0;     //Faixa de frequencia analisada
    while (true)   // Esse while obtem o valor de J
    {
        f = F / 2;
        if ((lower >= 0 && high > f) || (lower >= f && high > F))
            break;
        F = f;
        J++;
    }

    if (wt != NULL)
        wt_free(wt);

    int N = 4096;                   //Quantidade de amostras que chegam a cada buffer de 170ms
    wt = wt_init(obj, "dwt", N, J); // Initialize the wavelet transform object
    setDWTExtension(wt, "sym");     // Options are "per" and "sym". Symmetric is the default option
    setWTConv(wt, "direct");
}

void FrequencyListener::SetCallback(FrequencyCallback callback, void* arg)
{
    this->callback = callback;
    this->arg = arg;
}

void FrequencyListener::SetThreshold(double thresh)
{
    this->thresh = thresh;
}

void FrequencyListener::process(const int &nbOfChannels, const int &nbOfSamplesByChannel, const AL_SOUND_FORMAT *buffer, const AL::ALValue &timeStamp)
{
    if (wt == NULL)
        return;

    //-------------------------------------------------------------------------//

    // DWT utilizando a biblioteca wavelib.h

    double inp[nbOfSamplesByChannel];
    int N, i;
    N = min(nbOfSamplesByChannel, 4096);

    int channelInd = 0;
    for (int i = 0; i < N; i++)
    {
        inp[i] = buffer[nbOfSamplesByChannel * channelInd + i];
    }

    dwt(wt, inp);

    //-------------------------------------------------------------------------//

    //Aqui é calculado o RMS --------------------------------------------------//

    int start, end;

    start = wt->length[0];
    end = start + wt->length[1];

    double rms = 0;
    double scale = 0.000030517578125;
    scale *= scale;

    for (int k = start; k < end; k++)
        rms += scale * (wt->output[k] * wt->output[k]);

    rms /= end - start;
    rms = sqrt(rms);
    //----------------------------------------------------------------------------//

    //Aqui é calculado o tempo do apito detectado
    if (rms >= thresh)
    {
        duration += 0.17;
        if (!detecting)
            detecting = true;
    }
    else if (duration > 0)
    {
        if (detecting)
        {
            if (callback != NULL)
                callback(arg, duration);
            duration = 0;
            detecting = false;
        }
    }
}
