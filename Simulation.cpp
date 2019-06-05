#include "Simulation.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <cfloat>

Simulation::Simulation()
{
    srand (time(NULL));
    pose[0] = pose[1] = 0;
    pose[2] = 0;
    origin[0] = origin[1] = 0;
    size[0] = size[1] = 1000;
    aperture = Deg2Rad(30);
    maxDistance = 500;
    stdPose[0] = 2;
    stdPose[1] = 2;
    stdPose[2] = Deg2Rad(1);
    stdSensor[0] = 10;
    stdSensor[1] = Deg2Rad(3);
    stdMotion[0] = 0.01;
    stdMotion[1] = 0.01;
    randnMean = Mat::zeros(1,1,CV_64FC1);
    randnSigma = Mat::ones(1,1,CV_64FC1);
    randnContainer = Mat::zeros(1,1,CV_64FC1);
    I.setIdentity();
    Covariances();
}

Simulation::~Simulation()
{

}

void Simulation::Covariances()
{
    Q.setZero();
    Q(0,0) = stdPose[0]*stdPose[0];
    Q(1,1) = stdPose[1]*stdPose[1];
    Q(2,2) = stdPose[2]*stdPose[2];
    P = Q;
    M.setZero();
    M(0,0) = stdMotion[0]*randn();
    M(0,0) *= M(0,0);
    M(1,1) = stdMotion[1]*randn();
    M(1,1) *= M(1,1);
    R.setZero();
    R(0,0) = stdSensor[0]*stdSensor[0];
    R(1,1) = stdSensor[1]*stdSensor[1];
}

void Simulation::CalculateCorners()
{
    corners[0] = -origin[0];
    corners[1] = -origin[1];
    corners[2] = size[0]-origin[0];
    corners[3] = size[1]-origin[1];
}

void Simulation::SetOrigin(float x, float y)
{
    origin[0] = x;
    origin[1] = y;
    CalculateCorners();
}

void Simulation::SetOrigin(float *origin)
{
    this->origin[0] = origin[0];
    this->origin[1] = origin[1];
    CalculateCorners();
}

void Simulation::GetOrigin(float *x, float *y)
{
    *x = origin[0];
    *y = origin[1];
}

void Simulation::GetOrigin(float *origin)
{
    origin[0] = this->origin[0];
    origin[1] = this->origin[1];
}

void Simulation::SetSize(float w, float h)
{
    size[0] = w;
    size[1] = h;
    CalculateCorners();
}

void Simulation::SetSize(float *size)
{
    this->size[0] = size[0];
    this->size[1] = size[1];
    CalculateCorners();
}

void Simulation::GetSize(float *w, float *h)
{
    *w = size[0];
    *h = size[1];
}

void Simulation::GetSize(float *size)
{
    size[0] = this->size[0];
    size[1] = this->size[1];
}

void Simulation::SetPose(float x, float y, float th)
{
    pose[0] = x;
    pose[1] = y;
    pose[2] = th;
}

void Simulation::SetPose(float *pose)
{
    this->pose[0] = pose[0];
    this->pose[1] = pose[1];
    this->pose[2] = pose[2];
}

void Simulation::GetPose(float *x, float *y, float *th)
{
    *x = pose[0];
    *y = pose[1];
    *th = pose[2];
}

void Simulation::GetPose(float *pose)
{
    pose[0] = this->pose[0];
    pose[1] = this->pose[1];
    pose[2] = this->pose[2];
}

float Simulation::Error(float *pose)
{
    float dx = pose[0] - this->pose[0];
    float dy = pose[1] - this->pose[1];
    return sqrtf(dx*dx + dy*dy);
}

void Simulation::SetAperture(float aperture)
{
    this->aperture = aperture;
}

float Simulation::GetAperture()
{
    return aperture;
}

void Simulation::SetMaxDistance(float maxDistance)
{
    this->maxDistance = maxDistance;
}

float Simulation::GetMaxDistance()
{
    return maxDistance;
}

void Simulation::SetStdPose(float x, float y, float th)
{
    stdPose[0] = x;
    stdPose[1] = y;
    stdPose[2] = th;
}

void Simulation::SetStdPose(float *stdPose)
{
    this->stdPose[0] = stdPose[0];
    this->stdPose[1] = stdPose[1];
    this->stdPose[2] = stdPose[2];
}

void Simulation::GetStdPose(float *x, float *y, float *th)
{
    *x = stdPose[0];
    *y = stdPose[1];
    *th = stdPose[2];
}

void Simulation::GetStdPose(float *stdPose)
{
    stdPose[0] = this->stdPose[0];
    stdPose[1] = this->stdPose[1];
    stdPose[2] = this->stdPose[2];
}

void Simulation::SetStdMotion(float p, float th)
{
    stdMotion[0] = p;
    stdMotion[1] = th;
}

void Simulation::SetStdMotion(float *stdMotion)
{
    this->stdMotion[0] = stdMotion[0];
    this->stdMotion[1] = stdMotion[1];
}

void Simulation::GetStdMotion(float *p, float *th)
{
    *p = stdMotion[0];
    *th = stdMotion[1];
}

void Simulation::GetStdMotion(float *stdMotion)
{
    stdMotion[0] = this->stdMotion[0];
    stdMotion[1] = this->stdMotion[1];
}

void Simulation::SetStdSensor(float p, float th)
{
    stdSensor[0] = p;
    stdSensor[1] = th;
}

void Simulation::SetStdSensor(float *stdSensor)
{
    this->stdSensor[0] = stdSensor[0];
    this->stdSensor[1] = stdSensor[1];
}

void Simulation::GetStdSensor(float *p, float *th)
{
    *p = stdSensor[0];
    *th = stdSensor[1];
}

void Simulation::GetStdSensor(float *stdSensor)
{
    stdSensor[0] = this->stdSensor[0];
    stdSensor[1] = this->stdSensor[1];
}

void Simulation::Update(float *command, float dt, vector<FEATURE> &features, Mat &img)
{
    float ds = dt*command[0];
    float dw = dt*command[1];

    xk(0) = pose[0];
    xk(1) = pose[1];
    xk(2) = pose[2];

    uk(0) = ds;
    uk(1) = dw;

    xk(0) += ds*cosf(xk(2) + 0.5f*dw);
    xk(1) += ds*sinf(xk(2) + 0.5f*dw);
    xk(2) += dw;

    Fk(0,0) = 1;
    Fk(1,0) = 0;
    Fk(2,0) = 0;
    Fk(0,1) = 0;
    Fk(1,1) = 1;
    Fk(2,1) = 0;
    Fk(0,2) = -ds*sinf(xk(2) + 0.5f*dw);
    Fk(1,2) =  ds*cosf(xk(2) + 0.5f*dw);
    Fk(2,2) = 1;

    Gk(0,0) = cosf(xk(2) + 0.5f*dw);
    Gk(1,0) = sinf(xk(2) + 0.5f*dw);
    Gk(2,0) = 0;
    Gk(0,1) = -ds*0.5f*sinf(xk(2) + 0.5f*dw);
    Gk(1,1) =  ds*0.5f*cosf(xk(2) + 0.5f*dw);
    Gk(2,1) = 1;

    P = Fk*P*Fk.transpose() + Gk*M*Gk.transpose() + Q;
    
    vector<FEATURE> globalFeatures;
    CorrelateFeatures(features, globalFeatures);
    DrawFeatures(img, pose, globalFeatures, Scalar(255, 0, 0));

    Vector3f xk_p = xk;
    FEATURE f;
    for(int i = 0; i < features.size(); i++)
    {
        f = globalFeatures[i];
        y(0) = features[i].distance - f.distance;
        y(1) = features[i].angle - f.angle;

        H.setZero();
        H(0,0) = -(f.pose[0] - xk_p(0))/f.distance;
        H(0,1) = -(f.pose[1] - xk_p(1))/f.distance;
        H(1,0) =  (f.pose[0] - xk_p(0))/(f.distance*f.distance);
        H(1,1) = -(f.pose[1] - xk_p(1))/(f.distance*f.distance);
        H(1,2) = -1;

        S = H*P*H.transpose() + R;
        K = P*H.transpose()*S.inverse();
        
        xk = xk + K*y;
        xk(2) = FixAngle(xk(2));

        P = (I - K*H)*P;
    }

    pose[0] = xk(0);
    pose[1] = xk(1);
    pose[2] = FixAngle(xk(2));
}   

void Simulation::AddToMap(float x1, float y1, float x2, float y2, bool offset)
{
    ELEMENT line;
    line.type = ELEMENT_LINE;
    line.x1 = x1;
    line.y1 = y1;
    line.x2 = x2;
    line.y2 = y2;
    AddToMap(line, offset);
}

void Simulation::AddToMap(float x1, float y1, float radius, bool offset)
{
    ELEMENT line;
    line.type = ELEMENT_CIRCLE;
    line.x1 = x1;
    line.y1 = y1;
    line.radius = radius;
    AddToMap(line, offset);
}

void Simulation::AddToMap(ELEMENT element, bool offset)
{
    if(offset)
    {
        element.x1 -= origin[0];
        element.y1 -= origin[1];
        element.x2 -= origin[0];
        element.y2 -= origin[1];
    }
    map.push_back(element);
}

void Simulation::SetMap(int n, ELEMENT *elements)
{
    map.clear();
    for(int i = 0; i < n; i++)
        map.push_back(elements[i]);
}

bool Simulation::LoadMap(string fileName)
{
    ifstream file(fileName.c_str());
    if(!file.good())
        return false;
    map.clear();
    file >> size[0] >> size[1];
    file >> origin[0] >> origin[1];
    CalculateCorners();
    while(!file.eof())
    {
        ELEMENT element;
        file >> element.type;
        switch(element.type)
        {
            case ELEMENT_LINE:
                file >> element.x1 >> element.y1 >> element.x2 >> element.y2;
                break;
            case ELEMENT_CIRCLE:
                file >> element.x1 >> element.y1 >> element.radius;
                break;
            default:
                break;
        }
        if(!file.eof())
            map.push_back(element);
    }
}

bool Simulation::SaveMap(string fileName)
{
    ofstream file(fileName.c_str());
    file << size[0] << "\t" << size[1] << endl;
    file << origin[0] << "\t" << origin[1] << endl;
    ELEMENT element;
    for(int i = 0; i < map.size(); i++)
    {
        element = map[i];
        file << element.type;
        switch(element.type)
        {
            case ELEMENT_LINE:
                file << "\t" << element.x1 << "\t" << element.y1 << "\t" << element.x2 << "\t" << element.y2;
                break;
            case ELEMENT_CIRCLE:
                file << "\t" << element.x1 << "\t" << element.y1 << "\t" << element.radius;
                break;
            default:
                break;
        }
        file << endl;
    }
}

float Simulation::FixAngle(float rad)
{
    rad = std::fmod(rad, 6.28318530718f);
    if (rad > 3.14159265359f)
        rad -= 6.28318530718f;
    return rad;
}

float Simulation::randn() 
{
    /*
    double u = ((double) rand() / (RAND_MAX)) * 2 - 1;
    double v = ((double) rand() / (RAND_MAX)) * 2 - 1;
    double r = u * u + v * v;
    if (r == 0 || r > 1) return randn();
    double c = sqrt(-2 * log(r) / r);
    return u * c;
    */
    cv::randn(randnContainer,  randnMean, randnSigma);
    return (float)randnContainer.at<double>(0);
}

float Simulation::random()
{
    return (rand() % 1000)*0.001f;
}

void Simulation::Draw(Mat &img, Scalar mapColor, Scalar poseColor, Scalar particlesColor)
{
    Point o, d;

    for(int i = 0; i < map.size(); i++)
    {
        o.x = map[i].x1 + origin[0];
        o.y = map[i].y1 + origin[1];
        switch(map[i].type)
        {
            case ELEMENT_LINE:
                d.x = map[i].x2 + origin[0];
                d.y = map[i].y2 + origin[1];
                line(img, o, d, mapColor, 2);
                break;
            case ELEMENT_CIRCLE:
                circle(img, o, map[i].radius, mapColor, 2);
                break;
            default:
                break;
        }
    }

    o.x = pose[0] + origin[0];
    o.y = pose[1] + origin[1];
    circle(img, o, 10, poseColor, 1);
    d.x = 12*cosf(pose[2]) + o.x;
    d.y = 12*sinf(pose[2]) + o.y;
    line(img, o, d, poseColor, 2);
}

void Simulation::Draw(Mat &img, float *realPose, Scalar realPoseColor, Scalar mapColor, Scalar poseColor, Scalar particlesColor)
{
    Draw(img, mapColor, poseColor, particlesColor);

    Point o, d;
    o.x = realPose[0] + origin[0];
    o.y = realPose[1] + origin[1];
    circle(img, o, 10, realPoseColor, 1);
    d.x = 12*cosf(realPose[2]) + o.x;
    d.y = 12*sinf(realPose[2]) + o.y;
    line(img, o, d, realPoseColor, 2);
}

void Simulation::DrawFeatures(Mat &img, float *pose, vector<FEATURE> &features, Scalar featuresColor)
{
    Point o, d;
    if(pose != NULL)
    {
        o.x = pose[0] + origin[0];
        o.y = pose[1] + origin[1];
    }
    for(int i = 0; i < features.size(); i++)
    {
        if(features[i].distance >= maxDistance || features[i].distance <= 0)
            continue;
        d.x = features[i].pose[0] + origin[0];
        d.y = features[i].pose[1] + origin[1];
        circle(img, d, 5, featuresColor, CV_FILLED);   
        if(pose != NULL)
            line(img, o, d, featuresColor, 1);
    }
}

void Simulation::FindFeatures(float *pose, vector<FEATURE> &features, bool applyError)
{
    if(this->features.size() == 0)
    {
        LoadFeatures();
    }

    features.clear();

    FEATURE feature;
    float distance, dx, dy, angle;
    for(int i = 0; i < this->features.size(); i++)
    {
        feature = this->features[i];
        feature.distance = maxDistance;
        dx = feature.pose[0] - pose[0];
        dy = feature.pose[1] - pose[1];
        distance = sqrtf(dx*dx + dy*dy);
        if(distance > maxDistance)
        {
            //features.push_back(feature);
            continue;    
        }
        angle = FixAngle(atan2f(dy, dx)-pose[2]);
        if(abs(angle) > aperture)
        {
            //features.push_back(feature);
            continue;    
        }
        feature.angle = angle;
        feature.distance = distance;
        if(applyError)
        {
            feature.distance += stdSensor[0]*randn();
            feature.angle += stdSensor[1]*randn();
            feature.angle = FixAngle(feature.angle);
        }
        features.push_back(feature);
    }
}

void Simulation::LoadFeatures()
{
    features.clear();
    ELEMENT el1, el2;
    float angle, dist, nearest[2], dx, dy, d;
    Point2f intersection;
    bool add;
    for(int i = 0; i < map.size(); i++)
    {
        el1 = map[i];
        switch(el1.type)
        {
            case ELEMENT_CIRCLE:
            {
                FEATURE feature;
                feature.type = FEATURE_CIRCLE;
                feature.pose[0] = el1.x1;
                feature.pose[1] = el1.y1;
                feature.radius = el1.radius;
                features.push_back(feature);
            }
            break;
            case ELEMENT_LINE:
            {
                for(int j = 0; j < map.size(); j++)
                {
                    if(i == j)
                        continue;
                    el2 = map[j];
                    switch(el2.type)
                    {
                        case ELEMENT_CIRCLE:
                        {
                            dist = Point2Line(el1, el2, nearest);
                            if(dist < el2.radius)
                            {
                                FEATURE feature;
                                feature.type = FEATURE_CORNER;
                                feature.pose[0] = nearest[0];
                                feature.pose[1] = nearest[1];
                                feature.radius = 0;
                                features.push_back(feature);
                            }
                        }
                        break;
                        case ELEMENT_LINE:
                        {
                            intersection = ComputeIntersect(el1, el2);
                            if(intersection.x < FLT_MAX && intersection.y < FLT_MAX)
                            {
                                angle = GetLineAngle(el1, el2);
                                //cout << Rad2Deg(angle) << endl;
                                //if(!(abs(angle) < 0.1 || (abs(angle)-PI) < 0.1 || (abs(angle)-2*PI) < 0.1))
                                {
                                    add = true;
                                    for(int n = 0; n < features.size(); n++)
                                    {
                                        dx = features[n].pose[0] - intersection.x;
                                        dy = features[n].pose[1] - intersection.y;
                                        d = sqrt(dx*dx + dy*dy);
                                        if(d < 1)
                                        {
                                            add = false;
                                            break;
                                        }
                                    }
                                    if(add)
                                    {
                                        FEATURE feature;
                                        feature.type = FEATURE_CORNER;
                                        feature.pose[0] = intersection.x;
                                        feature.pose[1] = intersection.y;
                                        feature.radius = el1.radius;
                                        features.push_back(feature);
                                    }
                                }
                            }
                        }
                        break;
                        default:
                            break;
                    }
                }
            }
            break;
            default:
                break;
        }
    }
}

/*
void Simulation::CorrelateFeatures(vector<FEATURE> &features, vector<FEATURE> &globalFeatures)
{
    float x1, y1, x2, y2, d, dx, dy, m;
    FEATURE f1, f2, ff;
    for(int i = 0; i < features.size(); i++)
    {
        m = 2*maxDistance;
        f1 = features[i];
        x1 = f1.pose[0];
        y1 = f1.pose[1];
        for(int j = 0; j < this->features.size(); j++)
        {
            f2 = this->features[j];
            x2 = f2.pose[0];
            y2 = f2.pose[1];
            dx = x2-x1;
            dy = y2-y1;
            d = sqrt(dx*dx + dy*dy);
            if(d < m)
            {
                m = d;
                ff.type = f2.type;
                ff.pose[0] = f2.pose[0];
                ff.pose[1] = f2.pose[1];
                dx = x2-pose[0];
                dy = y2-pose[1];
                ff.angle = FixAngle(atan2f(dy, dx) - pose[2]);
                ff.distance = sqrt(dx*dx + dy*dy);
            }
        }
        globalFeatures.push_back(ff);
    }
}
*/


void Simulation::CorrelateFeatures(vector<FEATURE> &features, vector<FEATURE> &globalFeatures)
{
    float x1, y1, x2, y2, d, dx, dy, m;
    FEATURE f1, f2, ff;
    int type1, type2;
    for(int i = 0; i < features.size(); i++)
    {
        m = 2*maxDistance;
        f1 = features[i];
        x1 = pose[0] + f1.distance*cos(pose[2] + f1.angle);
        y1 = pose[1] + f1.distance*sin(pose[2] + f1.angle);
        type1 = f1.type;
        for(int j = 0; j < this->features.size(); j++)
        {
            f2 = this->features[j];
            x2 = f2.pose[0];
            y2 = f2.pose[1];
            type2 = f2.type;
            dx = x2-x1;
            dy = y2-y1;
            d = sqrt(dx*dx + dy*dy);
            if(d < m && type1 == type2)
            {
                if(type1 == FEATURE_CORNER || abs(f1.radius - f2.radius) < 10)
                {
                    m = d;
                    ff.type = f2.type;
                    ff.pose[0] = f2.pose[0];
                    ff.pose[1] = f2.pose[1];
                    dx = x2-pose[0];
                    dy = y2-pose[1];
                    ff.angle = FixAngle(atan2f(dy, dx) - pose[2]);
                    ff.distance = sqrt(dx*dx + dy*dy);
                }
            }
        }
        globalFeatures.push_back(ff);
    }
}

float Simulation::GetLineAngle(ELEMENT &l1, ELEMENT &l2)
{
    Vec2f a, b;
    a(0) = l1.x2 - l1.x1;
    a(1) = l1.y2 - l1.y1;
    b(0) = l2.x2 - l2.x1;
    b(1) = l2.y2 - l2.y1;
    float angle = atan2(b(1), b(0)) - atan2(a(1), a(0));
    if (angle < 0) 
        angle += 6.28318530718f;
    angle = FixAngle(angle);
    return angle;
}

Point2f Simulation::ComputeIntersect(ELEMENT &a, ELEMENT &b)
{
    int x1 = a.x1, y1 = a.y1, x2 = a.x2, y2 = a.y2;
    int x3 = b.x1, y3 = b.y1, x4 = b.x2, y4 = b.y2;
    if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
    {
        cv::Point2f pt;
        pt.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
        pt.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;
        //-10 is a threshold, the POI can be off by at most 10 pixels
        if (pt.x < min(x1, x2) - 10 || pt.x > max(x1, x2) + 10 || pt.y < min(y1, y2) - 10 || pt.y > max(y1, y2) + 10)
        {
            return Point2f(FLT_MAX, FLT_MAX);
        }
        if (pt.x < min(x3, x4) - 10 || pt.x > max(x3, x4) + 10 || pt.y < min(y3, y4) - 10 || pt.y > max(y3, y4) + 10)
        {
            return Point2f(FLT_MAX, FLT_MAX);
        }
        return pt;
    }
    else
        return cv::Point2f(FLT_MAX, FLT_MAX);
}

float Simulation::Point2Line(ELEMENT line, ELEMENT point, float *nearest)
{
    Vec2f ap, ab, _c;
    double u, nm, dx, dy, d;
    Vec2f a;
    a(0) = line.x1;
    a(1) = line.y1;

    ap(0) = point.x1 - line.x1;
    ap(1) = point.y1 - line.y1;
    ab(0) = line.x2 - line.x1;
    ab(1) = line.y2 - line.y1;

    nm = ab(0) * ab(0) + ab(1) * ab(1);
    u = ap.dot(ab) / nm;

    _c = a + ab * u;
    nearest[0] = _c(0);
    nearest[1] = _c(1);

    dx = point.x1 - _c(0);
    dy = point.y1 - _c(1);
    d = sqrt(dx * dx + dy * dy);
    return d;
}

void Simulation::Simulate(float *pose, float *command, float dt, bool applyError)
{
    if(applyError)
    {
        float ds = dt*command[0];
        float dw = dt*command[1];
        pose[0] += ds*cosf(pose[2] + dw*0.5f) + stdPose[0]*randn();
        pose[1] += ds*sinf(pose[2] + dw*0.5f) + stdPose[1]*randn();
        pose[2] += dw + stdPose[2]*stdPose[2]*randn();
        pose[2] = FixAngle(pose[2]);
    }
    else
    {
        float ds = dt*command[0];
        float dw = dt*command[1];
        pose[0] += ds*cosf(pose[2] + dw*0.5f);
        pose[1] += ds*sinf(pose[2] + dw*0.5f);
        pose[2] += dw;
        pose[2] = FixAngle(pose[2]);
    }
}