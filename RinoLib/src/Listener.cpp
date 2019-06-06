#include "NaoLib/Audio/Listener.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>

#ifdef SOUNDPROCESSING_IS_REMOTE
#include <qi/application.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#endif

#include <signal.h>
#include <boost/shared_ptr.hpp>
#include <alcommon/albroker.h>
#include <alcommon/almodule.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/altoolsmain.h>

using namespace AL;
using namespace std;
using namespace Rinobot::Nao::Audio;

#ifdef SOUNDPROCESSING_IS_REMOTE
# define ALCALL
#else
# ifdef _WIN32
#  define ALCALL __declspec(dllexport)
# else
#  define ALCALL
# endif
#endif


extern "C"
{
  ALCALL int _createListenerModule(boost::shared_ptr<AL::ALBroker> pBroker)
  {
    AL::ALBrokerManager::setInstance(pBroker->fBrokerManager.lock());
    AL::ALBrokerManager::getInstance()->addBroker(pBroker);

    //AL::ALModule::createModule<Listener::ListenerProxy>(pBroker,"ListenerProxy");
    return 0;
  }

  ALCALL int _closeListenerModule()
  {
    return 0;
  }
}

Listener::ListenerProxy::ListenerProxy(boost::shared_ptr<ALBroker> pBroker, std::string pName, Listener *listener)
    : ALSoundExtractor(pBroker, pName), plistener(listener)
{

}

void Listener::ListenerProxy::init()
{
    // Do not call the function setClientPreferences in your constructor!
    // setClientPreferences : can be called after its construction!
    audioDevice->callVoid("setClientPreferences",
                        getName(),                //Name of this module
                        48000,                    //48000 Hz requested
                        (int)ALLCHANNELS,         //4 Channels requested
                        1                         //Deinterleaving requested
                        );
    startDetection();
}

Listener::ListenerProxy::~ListenerProxy()
{
    stopDetection();
}

void Listener::ListenerProxy::process(const int & nbOfChannels,
                                const int & nbOfSamplesByChannel,
                                const AL_SOUND_FORMAT * buffer,
                                const ALValue & timeStamp)
{
    plistener->process(nbOfChannels, nbOfSamplesByChannel, buffer, timeStamp);
}

Listener::Listener()
{
    this->connection = "";
    this->isModule = false;
}

bool Listener::InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker)
{
    if(Robot::IsConnected())
    {
        TMainType sig;
        sig = &_createListenerModule;

        //ALTools::mainFunction("alsoundprocessing", 1, argv, sig);

        try
        {
            proxy = new ListenerProxy(broker, "ListenerProxy", this);
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

Listener::~Listener()
{
    if(proxy != NULL)
    {
        delete proxy;
        proxy = NULL;
    }
}

bool Listener::Connect()
{
    if(isModule)
        return true;
    return false;
}

const bool Listener::IsConnected() const
{
    return (proxy != NULL);
}

const string Listener::Connection() const
{
    return connection;
}

void Listener::process(const int & nbOfChannels,
                                const int & nbOfSamplesByChannel,
                                const AL_SOUND_FORMAT * buffer,
                                const ALValue & timeStamp)
{
    
}