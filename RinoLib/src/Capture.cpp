#include "NaoLib/Vision/Capture.h"
#include "NaoLib/Utils/Timer.h"
#include <alvision/alimage.h>
#include <alvision/alvisiondefinitions.h>
#include <alerror/alerror.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>

using namespace AL;
using namespace std;
using namespace cv;
using namespace Rinobot::Nao::Utils;
using namespace Rinobot::Nao::Vision;

CaptureSettings::CaptureSettings()
{
    this->camera = CaptureSettings::BOTTOM;
    this->resolution = CaptureSettings::R320x240;
}

CaptureSettings::CaptureSettings(Camera camera)
{
    this->camera = camera;
    this->resolution = CaptureSettings::R320x240;
}

CaptureSettings::CaptureSettings(Resolution resolution)
{
    this->camera = CaptureSettings::BOTTOM;
    this->resolution = resolution;
}

CaptureSettings::CaptureSettings(Camera camera, Resolution resolution)
{
    this->camera = camera;
    this->resolution = resolution;
}

bool Capture::haveDisplay = true;

Capture::Capture()
{
    this->camProxy = NULL;
    this->capture = NULL;
    this->index = -1;
    this->ip = "";
    this->port = -1;
    this->file = "";
    this->connection = "";
    this->colorSpace = BGR;
    this->isModule = false;
    Capture::haveDisplay = true;
}

Capture Capture::Webcam(int index)
{
    Capture capture;
    capture.index = index;
    stringstream ss;
    ss << "LOCAL:" << index;
    capture.connection = ss.str();
    return capture;
}

Capture Capture::File(string file)
{
    Capture capture;
    capture.file = file;
    stringstream ss;
    ss << "LOCAL:" << file;
    capture.connection = ss.str();
    return capture;
}

Capture Capture::Ip(string ip, int port)
{
    Capture capture;
    capture.ip = ip;
    capture.port = port;
    stringstream ss;
    ss << ip << ":" << port;
    capture.connection = ss.str();
    return capture;
}

Capture Capture::Nao()
{
    Capture capture = Ip("127.0.0.1", 9559);
    Capture::haveDisplay = false;
    return capture;
}

Capture Capture::Open(string address, int port)
{
    bool isVideo = false;
    VideoCapture video(address);
    if(video.isOpened())
    {
        isVideo = true;
        video.release();
        return File(address);
    }
    return Ip(address, port);
}

bool Capture::InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker)
{
    CaptureSettings settings;
    return InitFromRobot(broker, &settings);
}

bool Capture::InitFromRobot(boost::shared_ptr<AL::ALBroker>& broker, RobotModuleSettings *settings)
{
    if(Robot::IsConnected())
    {
        try
        {
            CaptureSettings *cameraSettings = (CaptureSettings*)settings;
            camProxy = new ALVideoDeviceProxy(broker);
            clientName = camProxy->subscribeCamera("capture", (int)cameraSettings->camera, (int)cameraSettings->resolution, kBGRColorSpace, 30);
            connection = Robot::GetConnection();
            haveDisplay = Robot::IsRemote();
            this->isModule = true;
            return true;
        }
        catch(const AL::ALError& e)
        {
            cout << e.what() << endl;
            camProxy = NULL;
        }
    }
    return false;
}

void Capture::DestroyFromRobot()
{
    Disconnect();
}

Capture::~Capture()
{
    Disconnect();
}

void Capture::Disconnect()
{
    cout << "~Capture" << endl;
    if(capture != NULL)
    {
        cout << "Capture release" << endl;
        capture->release();
        delete capture;
        capture = NULL;
    }
    if(camProxy != NULL)
    {
        cout << "Capture unsubscribe" << endl;
        camProxy->unsubscribe(clientName);
        delete camProxy;
        camProxy = NULL;
    }
}

bool Capture::Connect()
{
    bool success = false;
    if(isModule)
        return true;
    if(index >= 0)
    {
        capture = new VideoCapture(index);
        success = capture->isOpened();
        if(!success)
        {
            capture->release();
            delete capture;
            capture = NULL;
        }
    }
    else if(!file.empty())
    {
        capture = new VideoCapture(file);
        success = capture->isOpened();
        if(!success)
        {
            capture->release();
            delete capture;
            capture = NULL;
        }
    }
    else if(port > 0 && !ip.empty())
    {
        try
        {
            camProxy = new ALVideoDeviceProxy(ip, port);
            clientName = camProxy->subscribeCamera("capture", AL::kBottomCamera, kQVGA, kBGRColorSpace, 30);
            success = true;
        }
        catch(const AL::ALError& e)
        {
            camProxy = NULL;
        }
    }
    return success;
}

const bool Capture::IsConnected() const
{
    return (camProxy != NULL || (capture != NULL && capture->isOpened()));
}

bool Capture::HaveDisplay()
{
    return Capture::haveDisplay;
}

bool Capture::Show(const std::string &name, const cv::Mat &img)
{
    if(HaveDisplay())
        imshow(name, img);
    return HaveDisplay();
}

int Capture::Wait(int ms)
{
    if(HaveDisplay())
        return waitKey(ms);
    else
    {
        Timer::Wait(ms);
        return 0;
    }
}

const string Capture::Connection() const
{
    return connection;
}

void Capture::SetColorSpace(ColorSpace colorSpace)
{
    this->colorSpace = colorSpace;
}

Mat Capture::Get()
{
    Mat frame;
    if(capture != NULL)
    {
        capture->read(frame);
    }
    else
    {
        try
        {
            ALValue img = camProxy->getImageRemote(clientName);
            int w = (int)img[0];
            int h = (int)img[1];
            frame = Mat(Size(w, h), CV_8UC3);
            //frame.data = (uchar*) img[6].GetBinary();
            const unsigned char* data = (uchar*) img[6].GetBinary();
            memcpy(frame.data, data, w*h*sizeof(unsigned char));
            camProxy->releaseImage(clientName);
        }
        catch(const AL::ALError& e)
        {
            std::cerr << "Caught exception " << e.what() << std::endl;
        }
    }
    if(colorSpace != BGR && !frame.empty())
    {
        switch(colorSpace)
        {
            case RGB:
                cvtColor(frame, frame, CV_BGR2RGB);
                break;
            case HSV:
                cvtColor(frame, frame, CV_BGR2HSV);
                break;
            case GRAY:
                cvtColor(frame, frame, CV_BGR2GRAY);
                break;
            default:
                break;
        }
    }
    if(!frame.empty())
        return frame.clone();
    return frame;
}