#ifndef _RINOBOT_NAO_LIB_UTIL_SHARED_MEMORY_H_
#define _RINOBOT_NAO_LIB_UTIL_SHARED_MEMORY_H_

#include <semaphore.h>
#include <string>

namespace Rinobot
{
    namespace Nao
    {
        namespace Utils
        {
            class SharedMemory
            {
                private:
                    std::string name;
                    int memoryHandle, memorySize;
                    sem_t *mutexHandle;
                    unsigned char *memoryData;
                public:
                    SharedMemory();

                    bool Create(std::string name, int size);
                    void Destroy();

                    bool IsReady();
                    std::string GetName();

                    bool Lock();
                    bool Unlock();
                    unsigned char *GetPtr();
            };
        }
    }
}

#endif