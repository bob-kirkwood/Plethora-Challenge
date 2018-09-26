#pragma once


#include "Vertices.h"
#include "Edges.h"


class Profile // corresponds to a JsonFile
    {
    private:
        VertexMap mapvert_;
        EdgeList lledges_;

    public:
        Profile(std::string strFile);

        const EdgeList getEdges() const { return lledges_; }

        const std::list<VertexMap::VtxPtr> getVertices()  { return mapvert_.getVertices(); }

        static Vertex::ID ReadID(Json::ValueIterator jiter)
            { // Most of the IDs are implemented as strings, but that might be an error
            // that is corrected in other files, so we try to read as in int first.
            try
                {
                return jiter.key().asInt();
                }
            catch (std::exception xx)
                { 
                return atoi(jiter.key().asString().c_str());
                }
            }

        int GetEvalCount() const;  // just for diagnostics

        double ComputeMatlArea() const; // Delivers our estimate, in dollars
        double ComputeCost() const; // Delivers our estimate, in dollars


        static double getMachineRate() { return 0.07; } // $/sec
        static double getMatlCost() { return 0.75; } // $/in^2
        static double getCutSpeed() { return 0.5; } // in/sec
        static double getPadding() { return 0.1; } // in

        static double fAccuracy__;
        static double getAccuracy() { return fAccuracy__; } // in
        static void setAccuracy(double val) { fAccuracy__ = val;  } // settable for scaleability test, default value 0.001 should work fine
    };

