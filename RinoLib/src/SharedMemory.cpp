#include <NaoLib/Utils/SharedMemory.h>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>

using namespace std;
using namespace Rinobot::Nao::Utils;

SharedMemory::SharedMemory()
{
    memoryData = (unsigned char*)MAP_FAILED;
    memoryHandle = -1;
    memorySize = 0;
    mutexHandle = SEM_FAILED;
    name = "";
}

bool SharedMemory::Create(string name, int size)
{
    if(memoryHandle != -1)
    {
        Destroy();
    }

    stringstream ss;
    ss << "/" << name;

    bool created = false;
    memoryHandle = shm_open(ss.str().c_str(), O_RDWR, S_IRUSR | S_IWUSR);
    if(memoryHandle == -1)
    {
        memoryHandle = shm_open(ss.str().c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        created = true;
    }
    if(memoryHandle == -1)
    {
        return false;
    }
    this->name = name;
    memorySize = size;
    if(created)
    {
        if(ftruncate(memoryHandle, size) == -1)
        {
            Destroy();
            return false;
        }
    }
    memoryData = (unsigned char*) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, memoryHandle, 0);
    if(memoryData == MAP_FAILED)
    {
        Destroy();
        return false;
    }
    if(created)
    {
        memset(memoryData, 0, size);
    }

    ss << "_mutex";
    created = false;
    mutexHandle = sem_open(ss.str().c_str(), 0, 0, 0);
    if(mutexHandle == SEM_FAILED)
    {
        mutexHandle = sem_open(ss.str().c_str(), O_CREAT, 0660, 0);
        created = true;
    }
    if(mutexHandle == SEM_FAILED)
    {
        Destroy();
        return false;
    }
    if(created)
    {
        if (sem_post (mutexHandle) == -1)
        {
            Destroy();
            return false;
        }
    }

    return true;
}

void SharedMemory::Destroy()
{
    if(memoryData != MAP_FAILED)
        munmap(memoryData, memorySize);
    memoryData = (unsigned char*)MAP_FAILED;
    if(memoryHandle != -1)
        close(memoryHandle);
    memoryHandle = -1;
    if(mutexHandle != SEM_FAILED)
    {
        sem_post (mutexHandle);
        sem_close(mutexHandle);
    }
    mutexHandle = SEM_FAILED;
}

bool SharedMemory::IsReady()
{
    return memoryHandle != -1;
}

string SharedMemory::GetName()
{
    return name;
}

bool SharedMemory::Lock()
{
    if(!IsReady())
        return false;
    if (sem_wait (mutexHandle) == -1)
        return false;
    return true;
}

bool SharedMemory::Unlock()
{
    if(!IsReady())
        return false;
    if (sem_post (mutexHandle) == -1)
        return false;
    return true;
}

unsigned char *SharedMemory::GetPtr()
{
    if(!IsReady())
        return NULL;
    return memoryData;
}