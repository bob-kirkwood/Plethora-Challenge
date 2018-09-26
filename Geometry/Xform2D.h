#pragma once

#include "Vertices.h"

class Xform2D
    {
    private:
        //std::array<std::array<double, 3>, 3> arr_; // [row] [col] ## ..for proper deletion
        //std::array<std::array<double, 3>, 3> arr_;
        double arr_[3][3];
    public:
        Xform2D();
        Xform2D(Xform2D& xfSrc)
            {
            for (int nxRow = 0; nxRow < 3; nxRow++)
                for (int nxCol = 0; nxCol < 3; nxCol++)
                    arr_[nxRow][nxCol] = xfSrc.arr_[nxRow][nxCol];
            }

        void set(int nRow, int nCol, double fVal)
            {
            arr_[nRow][nCol] = fVal;
            }

        void Rotate(double fAngle)
            { 
            Xform2D xf;
            double fsin = sin (fAngle);
            xf.arr_[0][0] = xf.arr_[1][1] = cos(fAngle);
            xf.arr_[0][1] = sin(fAngle);
            xf.arr_[1][0] = -xf.arr_[0][1];
            Concat(xf);
            }

        void Translate(double x, double y)
            {
            Xform2D xf;
            xf.arr_[2][0] += x;
            xf.arr_[2][1] += y;
            Concat(xf);
            }

        void Concat(Xform2D& xf)
            {
            Xform2D xfTmp(*this);
            arr_[0][0] = xfTmp.arr_[0][0] * xf.arr_[0][0] + xfTmp.arr_[0][1] * xf.arr_[1][0] + xfTmp.arr_[0][2] * xf.arr_[2][0];
            arr_[0][1] = xfTmp.arr_[0][0] * xf.arr_[0][1] + xfTmp.arr_[0][1] * xf.arr_[1][1] + xfTmp.arr_[0][2] * xf.arr_[2][1];
            arr_[0][2] = xfTmp.arr_[0][0] * xf.arr_[0][2] + xfTmp.arr_[0][1] * xf.arr_[1][2] + xfTmp.arr_[0][2] * xf.arr_[2][2];
            arr_[1][0] = xfTmp.arr_[1][0] * xf.arr_[0][0] + xfTmp.arr_[1][1] * xf.arr_[1][0] + xfTmp.arr_[1][2] * xf.arr_[2][0];
            arr_[1][1] = xfTmp.arr_[1][0] * xf.arr_[0][1] + xfTmp.arr_[1][1] * xf.arr_[1][1] + xfTmp.arr_[1][2] * xf.arr_[2][1];
            arr_[1][2] = xfTmp.arr_[1][0] * xf.arr_[0][2] + xfTmp.arr_[1][1] * xf.arr_[1][2] + xfTmp.arr_[1][2] * xf.arr_[2][2];
            arr_[2][0] = xfTmp.arr_[2][0] * xf.arr_[0][0] + xfTmp.arr_[2][1] * xf.arr_[1][0] + xfTmp.arr_[2][2] * xf.arr_[2][0];
            arr_[2][1] = xfTmp.arr_[2][0] * xf.arr_[0][1] + xfTmp.arr_[2][1] * xf.arr_[1][1] + xfTmp.arr_[2][2] * xf.arr_[2][1];
            arr_[2][2] = xfTmp.arr_[2][0] * xf.arr_[0][2] + xfTmp.arr_[2][1] * xf.arr_[1][2] + xfTmp.arr_[2][2] * xf.arr_[2][2];
            // Note: the above could be done with nested loops, but this is just as fast and it's more obvious what's happening
            }

        Position Apply(const Position& pos)
            {
            return Position
                (
                pos.getX() * arr_[0][0] + pos.getY() * arr_[1][0] + arr_[2][0],
                pos.getX() * arr_[0][1] + pos.getY() * arr_[1][1] + arr_[2][1]
                );
            }

    };

