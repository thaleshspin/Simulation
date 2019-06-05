cmake_minimum_required(VERSION 2.8)
project(km)

find_package(qibuild)
find_package (Eigen3 REQUIRED NO_MODULE)

include_directories(".")
include_directories(${EIGEN3_INCLUDE_DIRS})

qi_create_bin( simulation main.cpp Simulation.cpp)
qi_use_lib(simulation RinoLib)
#target_link_libraries (kalman Eigen3)