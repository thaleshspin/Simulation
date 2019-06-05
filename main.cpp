#include <iostream>
#include "Simulation.h"

//#define CONTROL

int main(int argc, char *argv[])
{
    Simulation simulation;
    float XR[3], XP[3], WR[2];
    vector<FEATURE> features;

    XR[0] = 0;
    XR[1] = -200.0f;
    XR[2] = 0;
    simulation.SetPose(XR);
    
    float V = 200.0f;
    float R = 200.0f;

    WR[0] = 0;
    WR[1] = 0;
    #ifndef CONTROL
    WR[0] = V;
    WR[1] = V/R;
    #endif

    Mat img = Mat::zeros(600, 900, CV_8UC3);

    simulation.SetAperture(Deg2Rad(30.48f));
    simulation.SetMaxDistance(800);
    simulation.LoadMap("map.txt");

    #ifdef CONTROL
    // ------- Controle
    float e = 1;
    float kp = 0.18f;
    float ka = 0.42f;
    float kb = -0.18f;
    float a, b, p, g;
    float dx, dy, dth;
    float XG[3];
    XG[0] = (simulation.random()*900) - 450.0f;
    XG[1] = (simulation.random()*600) - 300.0f;
    XG[2] = simulation.FixAngle(Deg2Rad(simulation.random()*360));
    // --------
    #endif

    float dt, ds, dw;
    int key;

    int DT = 30;
    dt = DT / 1000.0f;
    while(true)
    {   
        #ifdef CONTROL
        simulation.GetPose(XP);
        dx = XG[0] - XP[0];
        dy = XG[1] - XP[1];
        dth = XG[2] - XP[2];
        p = sqrt(dx*dx + dy*dy);
        if(p < e)
        {
            XG[0] = (simulation.random()*900) - 450.0f;
            XG[1] = (simulation.random()*600) - 300.0f;
            XG[2] = simulation.FixAngle(Deg2Rad(simulation.random()*360));
            dx = XG[0] - XP[0];
            dy = XG[1] - XP[1];
            dth = XG[2] - XP[2];
            p = sqrt(dx*dx + dy*dy);
        }
        cout << p << endl;
        g = atan2(dy, dx);
        a = simulation.FixAngle(g - XP[2]);
        b = simulation.FixAngle(XG[2] - g);
        WR[0] = min(kp*p, 10.0f);
        WR[1] = ka*a + kb*b;
        #endif

        img.setTo(Scalar(55, 160, 17));

        simulation.Simulate(XR, WR, dt, true);

        simulation.FindFeatures(XR, features, true);
        simulation.Update(WR, dt, features, img);

        simulation.GetPose(XP);
        cout << "Pose = [" << XP[0] << ", " << XP[1] << ", " << (XP[2] * 57.2958f) << "]" << endl;
        cout << "Error = " << simulation.Error(XR) << endl;

        simulation.Draw(img, XR);
        simulation.DrawFeatures(img, XR, features);
        imshow("System", img);

        key = waitKey(DT);
        if(key == 27)
            break;
    }


    return 0;
}