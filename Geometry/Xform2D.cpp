#include "stdafx.h"
#include "Xform2D.h"


Xform2D::Xform2D()
    {
    for (int nxRow = 0; nxRow < 3; nxRow++)
        for (int nxCol = 0; nxCol < 3; nxCol++)
            if (nxRow == nxCol)
                arr_[nxRow][nxCol] = 1;
            else
                arr_[nxRow][nxCol] = 0;
    }

