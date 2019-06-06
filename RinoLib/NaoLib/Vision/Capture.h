#ifndef _RINOBOT_NAO_LIB_VISION_CAPTURE_H_
#define _RINOBOT_NAO_LIB_VISION_CAPTURE_H_

#include <string>
#include <opencv2/opencv.hpp>
#include <alproxies/alvideodeviceproxy.h>
#include <alvision/alvisiondefinitions.h>
#include "NaoLib/Robot.h"

namespace Rinobot
{
    namespace Nao
    {
        namespace Vision
        {
            class CaptureSettings : public RobotModuleSettings
            {
                public:
                    enum Camera { TOP = AL::kTopCamera, BOTTOM = AL::kBottomCamera };
                    enum Resolution { R160x120 = AL::kQQVGA, R320x240 = AL::kQVGA, R640x480 = AL::kVGA, R1280x960 = AL::k4VGA };
                    Camera camera;
                    Resolution resolution;
                    CaptureSettings();
                    CaptureSettings(Camera);
                    CaptureSettings(Resolution);
                    CaptureSettings(Camera, Resolution);

            };

            // A classe Capture permite capturar imagens da câmera do robô, de um arquivo ou da webcam, ele pode funcionar como um módulo da classe Robot
            class Capture : public RobotModule
            {
                public:
                    enum ColorSpace { BGR = 0, RGB, HSV, GRAY };
                private:
                    AL::ALVideoDeviceProxy *camProxy;
                    std::string clientName;
                    cv::VideoCapture *capture;
                    std::string connection;
                    std::string ip;
                    std::string file;
                    int port;
                    int index;
                    bool isModule;
                    static bool haveDisplay;
                    ColorSpace colorSpace;
                public:
                    Capture();
                    static Capture Webcam(int = 0);
                    static Capture File(std::string);
                    static Capture Ip(std::string = "127.0.0.1", int = 9559);
                    static Capture Nao();
                    static Capture Open(std::string, int = 9559);
                    
                    bool InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker);
                    bool InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker, RobotModuleSettings *settings);
                    void DestroyFromRobot();

                    virtual ~Capture();
                    bool Connect();
                    void Disconnect();
                    const bool IsConnected() const;
                    static bool HaveDisplay();

                    const std::string Connection() const;

                    void SetColorSpace(ColorSpace);

                    cv::Mat Get();

                    static bool Show(const std::string&, const cv::Mat&);
                    static int Wait(int ms = 33);
            };
        }
    }
}

#endif