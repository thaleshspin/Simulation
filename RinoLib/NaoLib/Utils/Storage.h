#ifndef _RINOBOT_NAO_LIB_UTIL_STORAGE_H_
#define _RINOBOT_NAO_LIB_UTIL_STORAGE_H_

#include <string>
#include <sstream>
#include <map>
#include "NaoLib/Utils/Constants.h"

namespace Rinobot
{
    namespace Nao
    {
        namespace Utils
        {
            class Storage
            {
                private:
                    bool isOpened;
                    std::string fileName;
                    std::stringstream ss;
                    std::map<std::string, std::string> data;
                    void Read();
                    void Write();
                public:
                    Storage();
                    Storage(const std::string fileName);
                    Storage(const std::string path, const std::string fileName);

                    bool IsOpened();

                    bool Open(const std::string fileName);
                    bool Open(const std::string path, const std::string fileName);
                    void Close();

                    bool Get(const std::string key, float *value);
                    bool Get(const std::string key, int *value);
                    bool Get(const std::string key, std::string *value);
                    void Set(const std::string key, float value);
                    void Set(const std::string key, int value);
                    void Set(const std::string key, std::string value);

                    void Remove(const std::string key);
            };
        }
    }
}

#endif