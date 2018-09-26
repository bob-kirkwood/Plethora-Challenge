#pragma once

#include "Vertices.h"
#include "Xform2D.h"

class Edge // implements LineSegment per the Json schema
    {
    private: 
        std::string strType_; // not really needed
        Edge() {}; 

    protected:
        Vertex::ID id_;
        Vertex::ID idOwner_;
        std::list<VertexMap::VtxPtr> llVtxPtrs_; // This is how I often manage collections of 2
        Edge(Vertex::ID id) { id_ = id; }

    public:
        Edge(Vertex::ID id, Json::Value jval, const VertexMap mapvert);

        Vertex::ID getID() const { return id_; } 

        std::string getType() { return strType_; }
        void setType(std::string strType) { strType_ = strType;  }

        std::string ToStr() const; // just for diagnostics

        void ReadVerts(Json::Value jvalVerts, VertexMap mapvert);

        const Vertex& getVtx0() const { return *llVtxPtrs_.back(); } 
        const Vertex& getVtx1() const { return *llVtxPtrs_.front(); }

        virtual int GetEvalCount() const { return 2; } // see EdgeArc::GetEvalCount()

        virtual double TimeCut() const; // used in Profile to compute cost

        /*
        For ProcLimits, the args are the presumed limits.  If this instance is outside
        those limits, we update the limits.
        Xform2D allows the method to be called in a "What if it was rotated?" context
        There is more explaination in EdgeArc::ProcLimits (edges.cpp)
        */
        virtual void ProcLimits(double& xMin, double& xMax, double& yMin, double& yMax, Xform2D xf) const;

        static const std::string JsonTypeName() { return "LineSegment"; }

        static std::shared_ptr<Edge> ReadEdge(Json::ValueIterator& jiter, VertexMap mapvert);
    };

class EdgeList : public std::list<std::shared_ptr<Edge>> // Implements Edges per the Json schema
    {
    public:
        EdgeList() {}; // remove later
        EdgeList(Json::Value& jval, VertexMap mapvert);

        static std::string JsonTypeName() { return "Edges"; }
    };

class EdgeArc : public Edge // implements the Json CircularArc
    {
    private:
        bool bReverse_; // true means clockwise from vtx1, false: vtx0
        // Note: bReverse_ means we don't need to store the ClockwiseFrom vertex

        Position posCenter_;
        mutable std::list<Position> llChordPts_; // access via getChordPts() to assure that it's populated
        std::list<Position> getChordPts() const;

    public:
        EdgeArc(Vertex::ID id, Json::Value  jvalEdge, VertexMap mapvert);

        virtual int IsCircle() const { return 1 == llVtxPtrs_.size(); } 

        int GetEvalCount() const { return 2 + getChordPts().size(); }

        virtual double TimeCut() const;
        virtual void ProcLimits(double& xMin, double& xMax, double& yMin, double& yMax, Xform2D xf) const;

    public:
        static std::string JsonTypeName() { return "CircularArc"; }

    };
