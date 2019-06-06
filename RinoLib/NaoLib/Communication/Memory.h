#ifndef _RINOBOT_NAO_LIB_COMMUNICATION_MEMORY_H_
#define _RINOBOT_NAO_LIB_COMMUNICATION_MEMORY_H_

#include <string>
#include <vector>
#include <map>
#include <alproxies/almemoryproxy.h>
#include <alcommon/almodule.h>
#include "NaoLib/Robot.h"
#include "NaoLib/Utils/SharedMemory.h"
#include <semaphore.h>

namespace Rinobot
{
    namespace Nao
    {
        namespace Communication
        {
            enum MemoryMapTypes { MAP_NONE, MAP_INT, MAP_FLOAT, MAP_STRING, MAP_BYTES };
            typedef struct MemoryMapEntry_st
            {
                MemoryMapTypes type;
                int size;
                int offset;


                MemoryMapEntry_st()
                {
                    type = MAP_NONE;
                    size = 1;
                    offset = 0;
                }

                MemoryMapEntry_st(MemoryMapTypes tp)
                {
                    type = tp;
                    size = 1;
                    offset = 0;
                }

                MemoryMapEntry_st(MemoryMapTypes tp, int sz)
                {
                    type = tp;
                    size = sz;
                    offset = 0;
                }

            }MemoryMapEntry;

            typedef AL::ALValue MemoryValue;
            typedef void (*Event)(MemoryValue&);

            class MemoryMap : public std::map<std::string, MemoryMapEntry>
            {
                private:
                    std::string name;
                public:
                    MemoryMap(std::string);
                    const std::string Name();
            };

            class MemoryArray : public std::vector<MemoryValue>
            {
                public:
                    int add(const MemoryValue&);
                    bool remove(const MemoryValue&);
                    bool removeAt(const int);
            };

            // A classe Memory permite compartilhar memória entre os módulos
            class Memory : public RobotModule
            {
                private:
                    class MemoryProxy : public AL::ALModule
                    {
                        private:
                            Memory *pmemory;
                            AL::ALMemoryProxy *proxy;
                        public:
                            MemoryProxy(boost::shared_ptr<AL::ALBroker>& pBroker, std::string pName, Memory *memory, AL::ALMemoryProxy *proxy);
                            virtual ~MemoryProxy();
                            void init();
                            void callback(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg);
                    };

                    AL::ALMemoryProxy *proxy;
                    MemoryProxy *mProxy;
                    std::string connection;
                    bool isModule;
                    void callback(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg);
                    std::map<std::string, Event> callbacks;
                    std::vector<std::string> required;

                    MemoryMap memoryMap;
                    Rinobot::Nao::Utils::SharedMemory sharedMemory;
                    bool IsMapped(std::string);
                    template <class T>
                    T GetMappedMemory(std::string);
                    void SetMappedMemory(std::string, char*, int);
                    
                public:
                    Memory();
                    
                    bool InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker);

                    virtual ~Memory();
                    bool Connect();
                    const bool IsConnected() const;

                    const std::string Connection() const;

                    bool TryGet(const std::string, MemoryValue*);
                    bool TryGet(const std::string, std::string*);
                    bool TryGet(const std::string, float*);
                    bool TryGet(const std::string, int*);
                    MemoryValue Get(const std::string);
                    void *GetPtr(const std::string);
                    void Set(const std::string, MemoryValue&);
                    void Set(const std::string, float);
                    void Set(const std::string, int);
                    void Set(const std::string, std::string);
                    void Remove(const std::string);
                    bool TryGet(int, MemoryValue*);
                    bool TryGet(int, std::string*);
                    bool TryGet(int, float*);
                    bool TryGet(int, int*);
                    MemoryValue Get(int);
                    void *GetPtr(int);
                    void Set(int, MemoryValue&);
                    void Set(int, float);
                    void Set(int, int);
                    void Set(int, std::string);
                    void Remove(int);

                    bool GetGameControlData(RoboCupGameControlData*);

                    void CreateEvent(const std::string);
                    void Subscribe(const std::string, Event);
                    void Unsubscribe(const std::string);
                    void Publish(const std::string, MemoryValue&);

                    void Require(const std::string);
                    void Require(int);
                    void Require(const MemoryArray&);
                    bool CheckRequired();
                    void WaitRequired();


                    bool Map(MemoryMap &memoryMap);
                    void Unmap();
                    
            };
        }
    }
}

#endif