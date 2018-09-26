#pragma once

#include "json/json.h"

/*
This Position class would normally be called 'Point' or 'Vector'.
It's called position for consistency with the Json schema 
*/
class Position
    {
    private:
        double x_; 
        double y_;

    protected:
        void ProcPosition(Json::Value& jval);

    public:
        Position() { x_ = y_ = 0; };

        Position(Json::Value& jval) { ProcPosition(jval); }

        Position (const Position& posSrc)
            { // ..so default copy ctor would have worked, but I like to set a breakpoint and see it called
            x_ = posSrc.x_;
            y_ = posSrc.y_;
            }

        Position(double x, double y)
            {
            x_ = x;
            y_ = y;
            }

        double getX() const { return x_; }
        double getY() const { return y_; }

        double Magn() const { return sqrt(x_ * x_ + y_ * y_); }
        // so (posA - posB).Magn() is the distance from posA to posB

        friend Position operator-(Position posA, const Position& posB)
            {
            posA.x_ -= posB.getX();
            posA.y_ -= posB.getY();
            return posA;
            }

        friend Position operator+(Position posA, const Position& posB)
            {
            posA.x_ += posB.getX();
            posA.y_ += posB.getY();
            return posA;
            }

        /*
        For ProcLimits, the args are the presumed limits.  If this instance is outside
        those limits, we update them.
        */
        void ProcLimits(double& xMin, double& xMax, double& yMin, double& yMax) const;

        std::string ToStr() const; // only used for diagnostics

        /*
        ComputeAngle is for cases where Position really represents a 2D vector (aka a direction)
        I returns the angle from *this to dir
        */
        double ComputeAngle(Position& dir) const;
    };


