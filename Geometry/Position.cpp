#include "stdafx.h"

#include "Position.h"

using namespace  std;

void Position::ProcPosition(Json::Value& jval)
    {
    x_ = jval["X"].asDouble();
    y_ = jval["Y"].asDouble();
    }

string Position::ToStr() const
    {
    ostringstream ssOut;
    ssOut << "Position: " << x_ << ' ' << y_;
    return ssOut.str();
    }

void Position::ProcLimits(double& xMin, double& xMax, double& yMin, double& yMax) const
    {
    xMin = min(xMin, getX());
    xMax = max(xMax, getX());
    yMin = min(yMin, getY());
    yMax = max(yMax, getY());
    }

double Position::ComputeAngle(Position& dir) const
    { // To test this: 
    // turn off the caching in EdgeArc::GetChordPts() and run with IceCreamCone.json

    // dot product result is betw 0 and 180 then repeats  ..actually the equiv in radians
    double angleDP = acos((getX() * dir.getX() + getY() * dir.getY()) / (Magn() * dir.Magn()));

    // cross product result is betw -90 and 90 then repeats
    double angleCP = asin((getX() * dir.getY() + getY() * dir.getX()) / (Magn() * dir.Magn()));

    if ((angleCP <= 0) && angleDP <= (M_PI / 2))
        return angleCP; // 4th quadrant
    else if ((angleCP <= 0) && angleDP < (M_PI / 2))
        return -angleCP; // 3rd quadrant
    else
        return angleDP; // 1st & 2nd quadrant
    }
