cmake_minimum_required(VERSION 2.8)
project(NaoLib)

find_package(qibuild)

include_directories(".")

# Wavelib ----------
include_directories(Tools/wavelib/src)
include_directories(Tools/wavelib/auxiliary)
include_directories(Tools/wavelib/header)

file(GLOB SRC_SRC
    "NaoLib/Tools/wavelib/src/*.h"
    "NaoLib/Tools/wavelib/src/*.c"
)

file(GLOB AUX_SRC
    "NaoLib/Tools/wavelib/auxiliary/*.h"
    "NaoLib/Tools/wavelib/auxiliary/*.c"
)

file(GLOB HEADER_SRC
    "NaoLib/Tools/wavelib/header/*.h"
    "NaoLib/Tools/wavelib/header/*.c"
)
# Wavelib ----------

set (SOURCE_FILES 
    NaoLib/Robot.h src/Robot.cpp
    NaoLib/Vision/Capture.h src/Capture.cpp
    NaoLib/Vision/VisualCompass.h src/VisualCompass.cpp
    NaoLib/Audio/Speech.h src/Speech.cpp
    NaoLib/Audio/Listener.h src/Listener.cpp
    NaoLib/Audio/FrequencyListener.h src/FrequencyListener.cpp
    NaoLib/Kinetics/Motion.h src/Motion.cpp
    NaoLib/Kinetics/Posture.h src/Posture.cpp
    NaoLib/Communication/Memory.h src/Memory.cpp
    NaoLib/Communication/Communicator.h src/Communicator.cpp
    NaoLib/Utils/Timer.h src/Timer.cpp
    NaoLib/Utils/Storage.h src/Storage.cpp
    NaoLib/Utils/SharedMemory.h src/SharedMemory.cpp
    NaoLib/Representations/RinobotMemoryMap.h src/RinobotMemoryMap.cpp
    ${SRC_SRC} ${AUX_SRC} ${HEADER_SRC}) 

qi_create_lib(NaoLib STATIC ${SOURCE_FILES})

qi_use_lib(NaoLib 
    OPENCV2_CORE 
    OPENCV2_HIGHGUI 
    OPENCV2_IMGPROC 
    OPENCV2_VIDEO 
    OPENCV2_OBJDETECT 
    ALCOMMON 
    ALPROXIES ALVISION ALAUDIO)

qi_install_header(KEEP_RELATIVE_PATHS 
    NaoLib/Robot.h
    NaoLib/Vision/Capture.h
    NaoLib/Vision/VisualCompass.h
    NaoLib/Audio/Speech.h
    NaoLib/Audio/Listener.h
    NaoLib/Audio/FrequencyListener.h
    NaoLib/Kinetics/Types.h
    NaoLib/Kinetics/Motion.h
    NaoLib/Kinetics/Posture.h
    NaoLib/Communication/Memory.h
    NaoLib/Communication/Communicator.h
    NaoLib/Utils/Timer.h
    NaoLib/Utils/Storage.h
    NaoLib/Utils/SharedMemory.h
    NaoLib/Utils/Constants.h
    NaoLib/GameControl/RoboCupGameControlData.h
    NaoLib/GameControl/SPLStandardMessage.h
    NaoLib/Representations/RinobotMemoryMap.h
    NaoLib/Tools/wavelib/header/wavelib.h)

qi_stage_lib(NaoLib)