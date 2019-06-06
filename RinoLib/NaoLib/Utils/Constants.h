#ifndef _RINOBOT_NAO_LIB_UTIL_CONSTANTS_H_
#define _RINOBOT_NAO_LIB_UTIL_CONSTANTS_H_

#include <string>

namespace Rinobot
{
    namespace Nao
    {
        namespace Utils
        {
            #define BASE_FOLDER std::string("~/rinobot/")
            #define VISION_FOLDER BASE_FOLDER+std::string("vision/")
            #define CONTROL_FOLDER BASE_FOLDER+std::string("control/")
            #define STRATEGY_FOLDER BASE_FOLDER+std::string("strategy/")
            #define DATA_FOLDER BASE_FOLDER+std::string("data/")
            
            #define FILE(path, fileName) path+fileName
        }
    }
}

#endif