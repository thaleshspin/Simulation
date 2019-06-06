#include <NaoLib/Representations/RinobotMemoryMap.h>
#include <NaoLib/Communication/Memory.h>
#include <iostream>

using namespace std;
using namespace Rinobot::Nao::Communication;

MemoryMap RinobotMemoryMap("rinobot_mem");

static const char* memoryKeyNames[] =
{
    // Behaviour
    "Rinobot/Behaviour/Running",
    // Strategy
    "Rinobot/Strategy/Position/x",
    "Rinobot/Strategy/Position/y",
    "Rinobot/Strategy/Position/theta",
    "Rinobot/Strategy/Player/Penalized",
    "Rinobot/Strategy/Player/Role",
    "Rinobot/Strategy/Player/Action",
    // Perception
    "Rinobot/Perception/Robot/Position/x",
    "Rinobot/Perception/Robot/Position/y",
    "Rinobot/Perception/Robot/Position/theta",
    "Rinobot/Perception/Ball/LocalPosition/distance",
    "Rinobot/Perception/Ball/LocalPosition/theta",
    "Rinobot/Perception/Ball/GlobalPosition/x",
    "Rinobot/Perception/Ball/GlobalPosition/y",
    "Rinobot/Perception/Ball/LastSeenTime",
    "Rinobot/Perception/Ball/IsSeen",
    // Control
    "Rinobot/Control/Velocity/x",
    "Rinobot/Control/Velocity/y",
    "Rinobot/Control/Velocity/theta",
    "Rinobot/Control/Player/IsFallen",
    // Calibration
    "Rinobot/Configs/Calibration/top/fx",
    "Rinobot/Configs/Calibration/top/fy",
    "Rinobot/Configs/Calibration/top/cx",
    "Rinobot/Configs/Calibration/top/cy",
    "Rinobot/Configs/Calibration/top/focusMM",
    "Rinobot/Configs/Calibration/bottom/fx",
    "Rinobot/Configs/Calibration/bottom/fy",
    "Rinobot/Configs/Calibration/bottom/cx",
    "Rinobot/Configs/Calibration/bottom/cy",
    "Rinobot/Configs/Calibration/bottom/focusMM",
    // Configs
    "Rinobot/Configs/BallDetector/top/cascade",
    "Rinobot/Configs/BallDetector/bottom/cascade"
};

static const MemoryMapTypes memoryKeyTypes[] =
{
    // Behaviour
    MAP_INT,
    // Strategy
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_INT,
    MAP_INT,
    MAP_INT,
    // Perception
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_INT,
    // Control
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_INT,
    // Calibration
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    MAP_FLOAT,
    // Configs
    MAP_STRING,
    MAP_STRING
};

static const int memoryKeySizes[] =
{
    // Behaviour
    0,
    // Strategy
    0,
    0,
    0,
    0,
    0,
    0,
    // Perception
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    // Control
    0,
    0,
    0,
    0,
    // Calibration
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    // Configs
    64,
    64
};

void LoadRinobotMemoryMap()
{
    if(RinobotMemoryMap.size() > 0)
        return;
    for(int i = Behaviour_Running; i < Memory_Key_Size; i++)
        RinobotMemoryMap[memoryKeyNames[i]] = MemoryMapEntry(memoryKeyTypes[i], memoryKeySizes[i]);
}

std::string GetMemoryKey(int key)
{
    if(key < 0 || key >= Memory_Key_Size)
        return "";
    return string(memoryKeyNames[key]);
}