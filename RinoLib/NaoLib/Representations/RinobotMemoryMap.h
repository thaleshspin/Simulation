#ifndef _RINOBOT_NAO_LIB_REPRESENTATIONS_RINOBOT_MEMORY_MAP_H_
#define _RINOBOT_NAO_LIB_REPRESENTATIONS_RINOBOT_MEMORY_MAP_H_

#include <NaoLib/Communication/Memory.h>
#include <string>

extern Rinobot::Nao::Communication::MemoryMap RinobotMemoryMap;

enum MemoryKeys
{
    // Behaviour
    Behaviour_Running = 0,
    // Strategy
    Strategy_Position_X,
    Strategy_Position_Y,
    Strategy_Position_Theta,
    Strategy_Player_Penalized,
    Strategy_Player_Role,
    Strategy_Player_Action,
    // Perception
    Perception_Robot_Position_X,
    Perception_Robot_Position_Y,
    Perception_Robot_Position_Theta,
    Perception_Ball_LocalPosition_Distance,
    Perception_Ball_LocalPosition_Theta,
    Perception_Ball_GlobalPosition_X,
    Perception_Ball_GlobalPosition_Y,
    Perception_Ball_LastSeenTime,
    Perception_Ball_IsSeen,
    // Control
    Control_Velocity_X,        
    Control_Velocity_Y,
    Control_Velocity_Theta,
    Control_Player_IsFallen,
    // Calibration
    Calibration_Top_Fx,
    Calibration_Top_Fy,
    Calibration_Top_Cx,
    Calibration_Top_Cy,
    Calibration_Top_Focus,
    Calibration_Bottom_Fx,
    Calibration_Bottom_Fy,
    Calibration_Bottom_Cx,
    Calibration_Bottom_Cy,
    Calibration_Bottom_Focus,
    // Configs
    Config_BallDetector_Top_Cascade,
    Config_BallDetector_Bottom_Cascade,

    // End
    Memory_Key_Size
};

extern std::string GetMemoryKey(int key);

#endif