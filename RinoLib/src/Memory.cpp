#include "NaoLib/Communication/Memory.h"
#include "NaoLib/Utils/Timer.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <limits.h>
#include <float.h>
#include <NaoLib/Representations/RinobotMemoryMap.h>

using namespace AL;
using namespace std;
using namespace Rinobot::Nao::Utils;
using namespace Rinobot::Nao::Communication;

int MemoryArray::add(const MemoryValue &value)
{
    this->push_back(value);
    return this->size()-1;
}

bool MemoryArray::remove(const MemoryValue &value)
{
    int sz = this->size();
    for(MemoryArray::iterator it = this->begin(); it != this->end(); )
    {
        if(*it == value)
            it = this->erase(it);
        else
            it++;
    }
    return (this->size() < sz);
}

bool MemoryArray::removeAt(const int position)
{
    if(position < this->size())
    {
        this->erase(this->begin() + position);
        return true;
    }
    return false;
}


MemoryMap::MemoryMap(std::string name)
{
    this->name = name;
}
const string MemoryMap::Name()
{
    return name;
}

Memory::MemoryProxy::MemoryProxy(boost::shared_ptr<AL::ALBroker>& broker, string pName, Memory *memory, ALMemoryProxy *proxy)
    : ALModule(broker, pName)
{
    this->proxy = proxy;
    this->pmemory = memory;
    functionName("callback", getName(), "");
    BIND_METHOD(Memory::MemoryProxy::callback);
}

void Memory::MemoryProxy::init()
{
    
}

Memory::MemoryProxy::~MemoryProxy()
{
    
}

void Memory::MemoryProxy::callback(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg)
{
    pmemory->callback(key, value, msg);
}

Memory::Memory()
    :   memoryMap("")
{
    this->proxy = NULL;
    this->mProxy = NULL;
    this->connection = "";
    this->isModule = false;
}

bool Memory::InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker)
{
    if(Robot::IsConnected())
    {
        try
        {
            proxy = new ALMemoryProxy(broker);
            mProxy = new MemoryProxy(broker, "MemoryProxy", this, proxy);
            connection = Robot::GetConnection();
            this->isModule = true;
            return true;
        }
        catch(const AL::ALError& e)
        {
            cout << e.what() << endl;
            proxy = NULL;
            mProxy = NULL;
        }
    }
    return false;
}

Memory::~Memory()
{
    if(proxy != NULL)
    {
        delete proxy;
        proxy = NULL;
    }
    if(mProxy != NULL)
    {
        delete mProxy;
        mProxy = NULL;
    }
}

bool Memory::Connect()
{
    if(isModule)
        return true;
    return false;
}

const bool Memory::IsConnected() const
{
    return (mProxy != NULL);
}

const string Memory::Connection() const
{
    return connection;
}

void Memory::callback(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg)
{
    cout << key << endl;
    cout << value << endl;
    cout << msg << endl;
}

bool Memory::TryGet(int key, MemoryValue *value)
{
    return TryGet(GetMemoryKey(key), value);
}

bool Memory::TryGet(const std::string key, MemoryValue *value)
{
    if(IsMapped(key))
    {
        MemoryMapEntry entry = memoryMap[key];
        switch(entry.type)
        {
            case MAP_INT:
            {
                int v = GetMappedMemory<int>(key);
                *value = v;
                return v != INT_MAX;
            }
            break;
            case MAP_FLOAT:
            {
                float v = GetMappedMemory<float>(key);
                *value = v;
                return v != FLT_MAX;
            }
            break;
            case MAP_STRING:
            {
                char *v = GetMappedMemory<char*>(key);
                int sz = *((int*)v);
                v = v + 4;
                string str(v, sz);
                *value = str;
                return sz > 0;
            }
            break;
            case MAP_BYTES:
            {
                unsigned char *v = GetMappedMemory<unsigned char*>(key);
                int sz = *((int*)v);
                v = v + 4;
                value->SetBinary((void*)v, sz);
                return sz > 0;
            }
            break;
            default:
                break;
        }
    }
    try
    {
        *value = Get(key);
        return true;
    }
    catch (const AL::ALError &e)
    {}
    return false;
}

bool Memory::TryGet(int key, std::string *value)
{
    return TryGet(GetMemoryKey(key), value);
}

bool Memory::TryGet(const std::string key, std::string *value)
{
    MemoryValue v;
    bool ret = TryGet(key, &v);
    if(ret)
    {
        std::string str = v;
        *value = str;
    }
    return ret;
}

bool Memory::TryGet(int key, float *value)
{
    return TryGet(GetMemoryKey(key), value);
}

bool Memory::TryGet(const std::string key, float *value)
{
    MemoryValue v;
    bool ret = TryGet(key, &v);
    if(ret)
        *value = v;
    return ret;
}

bool Memory::TryGet(int key, int *value)
{
    return TryGet(GetMemoryKey(key), value);
}

bool Memory::TryGet(const std::string key, int *value)
{
    MemoryValue v;
    bool ret = TryGet(key, &v);
    if(ret)
        *value = v;
    return ret;
}

MemoryValue Memory::Get(int key)
{
    return Get(GetMemoryKey(key));
}

MemoryValue Memory::Get(const std::string key)
{
    if(!IsConnected())
        return 0;
    if(IsMapped(key))
    {
        MemoryValue value;
        MemoryMapEntry entry = memoryMap[key];
        switch(entry.type)
        {
            case MAP_INT:
            {
                int v = GetMappedMemory<int>(key);
                value = v;
                return value;
            }
            break;
            case MAP_FLOAT:
            {
                float v = GetMappedMemory<float>(key);
                value = v;
                return value;
            }
            break;
            case MAP_STRING:
            {
                char *v = GetMappedMemory<char*>(key);
                int sz = *((int*)v);
                v = v + 4;
                string str(v, sz);
                value = str;
                return value;
            }
            break;
            case MAP_BYTES:
            {
                unsigned char *v = GetMappedMemory<unsigned char*>(key);
                int sz = *((int*)v);
                v = v + 4;
                value = MemoryValue((void*)v, sz);
                return value;
            }
            break;
            default:
                break;
        }
    }
    return proxy->getData(key);
}

void *Memory::GetPtr(int key)
{
    return GetPtr(GetMemoryKey(key));
}

void *Memory::GetPtr(const std::string key)
{
    if(!IsConnected())
        return NULL;
    if(IsMapped(key))
    {
        MemoryMapEntry entry = memoryMap[key];
        unsigned char *memoryData = sharedMemory.GetPtr();
        return ((void*)(&memoryData[entry.offset]));
    }
    return proxy->getDataPtr(key);
}

void Memory::Set(int key, MemoryValue &value)
{
    Set(GetMemoryKey(key), value);
}

void Memory::Set(const std::string key, MemoryValue &value)
{   
    if(!IsConnected())
        return;
    if(IsMapped(key))
    {
        MemoryMapEntry entry = memoryMap[key];
        switch(entry.type)
        {
            case MAP_INT:
            {
                int v = value;
                SetMappedMemory(key, (char*)&v, 4);
                return;
            }
            break;
            case MAP_FLOAT:
            {
                float v = value;
                SetMappedMemory(key, (char*)&v, 4);
                return;
            }
            break;
            case MAP_STRING:
            {
                string str = value;
                SetMappedMemory(key, (char*)str.c_str(), str.size());
                return;
            }
            break;
            case MAP_BYTES:
            {
                const void *ptr = value.GetBinary();
                SetMappedMemory(key, (char*)ptr, value.getSize());
                return;
            }
            break;
            default:
                break;
        }
    }
    return proxy->insertData(key, value);
}

void Memory::Set(int key, float value)
{
    Set(GetMemoryKey(key), value);
}

void Memory::Set(const std::string key, float value)
{
    MemoryValue v = value;
    return Set(key, v);
}

void Memory::Set(int key, int value)
{
    Set(GetMemoryKey(key), value);
}

void Memory::Set(const std::string key, int value)
{
    MemoryValue v = value;
    return Set(key, v);
}

void Memory::Set(int key, std::string value)
{
    Set(GetMemoryKey(key), value);
}

void Memory::Set(const std::string key, std::string value)
{
    MemoryValue v = value;
    return Set(key, v);
}

void Memory::Remove(int key)
{
    Remove(GetMemoryKey(key));
}

void Memory::Remove(const std::string key)
{
    if(!IsConnected())
        return;
    if(IsMapped(key))
    {
        MemoryValue value;
        MemoryMapEntry entry = memoryMap[key];
        switch(entry.type)
        {
            case MAP_INT:
            {
                int v = INT_MAX;
                SetMappedMemory(key, (char*)&v, 4);
                return;
            }
            break;
            case MAP_FLOAT:
            {
                float v = FLT_MAX;
                SetMappedMemory(key, (char*)&v, 4);
                return;
            }
            break;
            case MAP_STRING:
            {
                string str = "";
                SetMappedMemory(key, (char*)str.c_str(), 0);
                return;
            }
            break;
            case MAP_BYTES:
            {
                char ptr[1];
                SetMappedMemory(key, (char*)ptr, 0);
                return;
            }
            break;
            default:
                break;
        }
    }
    try
    {
        proxy->removeData(key);
    }
    catch (const AL::ALError &e)
    {}
}

bool Memory::GetGameControlData(RoboCupGameControlData *gameControlData)
{
    if(!IsConnected())
        return false;
    MemoryValue value = proxy->getData("GameCtrl/RoboCupGameControlData");
    if (value.isBinary() && value.getSize() == sizeof(RoboCupGameControlData))
    {
        memcpy(gameControlData, value, sizeof(RoboCupGameControlData));
    }
    return true;
}

void Memory::CreateEvent(const string name)
{
    if(!IsConnected())
        return;
    proxy->declareEvent(name);
}

void Memory::Subscribe(const string name, Event func)
{
    if(!IsConnected())
        return;
    proxy->subscribeToEvent(name, "MemoryProxy", "callback");
    callbacks[name] = func;
}

void Memory::Unsubscribe(const string name)
{
    if(!IsConnected())
        return;
    proxy->unsubscribeToEvent(name, "MemoryProxy");
    callbacks.erase(name);
}

void Memory::Publish(const string name, MemoryValue &value)
{
    if(!IsConnected())
        return;
    proxy->raiseEvent(name, value);
}

void Memory::Require(int req)
{
    required.push_back(GetMemoryKey(req));
}

void Memory::Require(const std::string req)
{
    required.push_back(req);
}

void Memory::Require(const MemoryArray &array)
{
    for(int i = 0; i < array.size(); i++)
    {
        if(array[i].isString())
            required.push_back(array[i]);
        else
            required.push_back(GetMemoryKey(array[i]));
    }
}

bool Memory::CheckRequired()
{
    MemoryValue value;
    for(int i = 0; i < required.size(); i++)
    {
        if(!TryGet(required[i], &value))
            return false;
    }
    return true;
}

void Memory::WaitRequired()
{
    while(true)
    {
        if(CheckRequired())
            break;
        Timer::Wait(100);
    }
}

bool Memory::Map(MemoryMap &memoryMap)
{
    this->memoryMap = memoryMap;
    int size = 0;
    for(MemoryMap::iterator it = this->memoryMap.begin(); it != this->memoryMap.end(); it++)
    {
        it->second.offset = size;
        switch(it->second.type)
        {
            case MAP_INT:
                size += sizeof(int);
                it->second.size = sizeof(int);
                break;
            case MAP_FLOAT:
                size += sizeof(float);
                it->second.size = sizeof(float);
                break;
            case MAP_STRING:
                size += it->second.size * sizeof(char) + sizeof(int);
                break;
            case MAP_BYTES:
                size += it->second.size * sizeof(unsigned char) + sizeof(int);
                break;
            default:
                break;
        }
    }

    return sharedMemory.Create(this->memoryMap.Name(), size);
}

void Memory::Unmap()
{
    sharedMemory.Destroy();
}

bool Memory::IsMapped(string key)
{
    if(!sharedMemory.IsReady())
        return false;
    if(memoryMap.find(key) == memoryMap.end() )
    {
        return false;
    }
    return true;
}

template <class T>
T Memory::GetMappedMemory(std::string key)
{
    T value;
    if(!sharedMemory.IsReady())
        return value;
    MemoryMapEntry entry = memoryMap[key];
    sharedMemory.Lock();
    unsigned char *memoryData = sharedMemory.GetPtr();
    value = *((T*)(&memoryData[entry.offset]));
    sharedMemory.Unlock();
    return value;
}

void Memory::SetMappedMemory(std::string key, char *value, int objSize)
{
    if(!sharedMemory.IsReady())
        return;
    MemoryMapEntry entry = memoryMap[key];
    int sz = 0;
    bool dual = false;
    switch(entry.type)
    {
        case MAP_INT:
        case MAP_FLOAT:
            sz = 4;
            break;
        case MAP_STRING:
        case MAP_BYTES:
            dual = true;
            sz = min(objSize, entry.size);
            break;
        default:
            break;
    }
    char *size = (char*)&sz;
    if (!sharedMemory.Lock())
    {
        cout << "SharedMemory Lock failed" << endl;
    }
    unsigned char *memoryData = sharedMemory.GetPtr();
    if(dual)
    {
        memcpy(&memoryData[entry.offset], size, 4);
        memcpy((&memoryData[entry.offset]) + 4, value, sz);
    }
    else
    {
        memcpy(&memoryData[entry.offset], value, sz);
    }
    if (!sharedMemory.Unlock())
    {
        cout << "SharedMemory Unlock failed" << endl;
    }
}