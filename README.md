# Simulation

This is a simulation that uses localization techniques applied to nao robots 

1. Install the SDK from Aldebaran

Download "C++ SDK 2.1.4 Linux 64" at https://community.aldebaran.com/en/resources/software

-> tar -zxvf naoqi-sdk-2.1.4.13-linux64.tar.gz

-> qibuild init

-> qitoolchain create nameOfYourToolchain path/to/your/sdk/toolchain.xml

-> qibuild add-config nameOfYourToolchain -t nameOfYourToolchain

2. Compile Rinolib -> python compileRinoLib.py

2. Follow all the steps at https://github.com/thaleshsp2/OpenCV-2.4.13-Compiler-HowTo

3. qibuild configure -c nameOfYourToolchain

4. qibuild make -c nameOfYourToolchain

5. run the simulator with ./build-nameOfYourToolchain/sdk/bin/simulation
