#ifndef _Simulation_H_
#define _Simulation_H_

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

using namespace std;
using namespace cv;
using namespace Eigen;

#define ELEMENT_LINE 0
#define ELEMENT_CIRCLE 1
#define FEATURE_CORNER 0
#define FEATURE_CIRCLE 1

#define PI 3.141592653589793f
#define Deg2Rad(d) (d*0.017453293f)
#define Rad2Deg(r) (r*57.295779513f)

typedef struct
{
    int type;
    float x1, y1, x2, y2, radius;
}ELEMENT;

typedef struct
{
    int type;
    float pose[2];
    float radius;
    float distance;
    float angle;
}FEATURE;

class Simulation
{
    private:
        float origin[2];
        float size[2];
        float pose[3];
        Vector3f xk;
        Vector2f uk, y;
        Matrix3f Fk, Q, P, I;
        Matrix2f M, R, S;
        Matrix<float, 3, 2> Gk, K;
        Matrix<float, 2, 3> H;
        float corners[4];
        float aperture, maxDistance;
        float stdPose[3];
        float stdMotion[2];
        float stdSensor[2];
        vector<ELEMENT> map;
        vector<FEATURE> features;
        Mat randnMean, randnSigma, randnContainer;
    public:
        
        Simulation();
        ~Simulation();

        void SetOrigin(float x, float y);
        void SetOrigin(float *origin);
        void GetOrigin(float *x, float *y);
        void GetOrigin(float *origin);

        void SetSize(float w, float h);
        void SetSize(float *size);
        void GetSize(float *w, float *h);
        void GetSize(float *size);

        void SetPose(float x, float y, float th);
        void SetPose(float *pose);
        void GetPose(float *x, float *y, float *th);
        void GetPose(float *pose);

        float Error(float *pose);

        void SetAperture(float aperture);
        float GetAperture();
        void SetMaxDistance(float maxDistance);
        float GetMaxDistance();

        void SetStdMotion(float p, float th);
        void SetStdMotion(float *stdMotion);
        void GetStdMotion(float *p, float *th);
        void GetStdMotion(float *stdMotion);

        void SetStdSensor(float p, float th);
        void SetStdSensor(float *stdSensor);
        void GetStdSensor(float *p, float *th);
        void GetStdSensor(float *stdSensor);
        
        void SetStdPose(float x, float y, float th);
        void SetStdPose(float *stdPose);
        void GetStdPose(float *x, float *y, float *th);
        void GetStdPose(float *stdPose);

        float FixAngle(float rad);
        float randn();
        float random();

        void Update(float *command, float dt, vector<FEATURE> &features, Mat &img);

        void AddToMap(float x1, float y1, float x2, float y2, bool offset);
        void AddToMap(float x1, float y1, float radius, bool offset);
        void AddToMap(ELEMENT element, bool offset);
        void SetMap(int n, ELEMENT *elements);
        bool LoadMap(string fileName);
        bool SaveMap(string fileName);
        void Draw(Mat &img, Scalar mapColor = Scalar(255, 255, 255), Scalar poseColor = Scalar(0, 0, 255), Scalar particlesColor = Scalar(196, 35, 27));
        void Draw(Mat &img, float *realPose, Scalar realPoseColor = Scalar(255, 0, 255), Scalar mapColor = Scalar(255, 255, 255), Scalar poseColor = Scalar(0, 0, 255), Scalar particlesColor = Scalar(196, 35, 27));

        void FindFeatures(float *pose, vector<FEATURE> &features, bool applyError);
        void DrawFeatures(Mat &img, float *pose, vector<FEATURE> &features,  Scalar featuresColor = Scalar(0, 255, 255));

        void Simulate(float *pose, float *command, float dt, bool applyError);
    private:
        void Covariances();
        void CalculateCorners();
        void CorrelateFeatures(vector<FEATURE> &features, vector<FEATURE> &globalFeatures);
        void LoadFeatures();
        float GetLineAngle(ELEMENT &l1, ELEMENT &l2);
        Point2f ComputeIntersect(ELEMENT &a, ELEMENT &b);
        float Point2Line(ELEMENT line, ELEMENT point, float *nearest);
};

#endif
