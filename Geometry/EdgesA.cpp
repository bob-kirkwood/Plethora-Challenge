#pragma once

#include "stdafx.h""

#include <list>

#include "geometry.h"
#include "vertices.h"

class Edge
    {
    private:
        Vertex vtx0_;
        Vertex vtx1_;
    };

class EdgeList : public std::list<Edge>
    {

    public:
        static std::string JsonTypeName() { return "Edges"; }

    };

class LineSegment
    {
    public:
        LineSegment();
        ~LineSegment();
    };



