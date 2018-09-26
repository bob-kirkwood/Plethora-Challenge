#include "stdafx.h"

#include "json/json.h"

#include "Edges.h"
#include "Vertices.h"
#include "Profile.h"
#include "Xform2D.h"

using namespace std;

shared_ptr<Edge> Edge::ReadEdge(Json::ValueIterator& jiter, VertexMap mapvert)
    {
    Vertex::ID id = Profile::ReadID(jiter); // Profile::IDfromString(jiter.key().asString());

    Json::Value jvalEdge = *jiter;

    string strTyp = jvalEdge["Type"].asString();

    if (Edge::JsonTypeName() == strTyp)
        return make_shared<Edge>(id, jvalEdge, mapvert);
    else if (EdgeArc::JsonTypeName() == strTyp)
        {
        shared_ptr<EdgeArc> sptr 
            = make_shared<EdgeArc>(id, jvalEdge, mapvert);
        return dynamic_pointer_cast<Edge>(sptr);
        }
    else
        throw exception("Unknown edge type");
    }

void Edge::ReadVerts(Json::Value jvalVerts, VertexMap mapvert)
    {
    switch (jvalVerts.size())
        {
        case 2:
            llVtxPtrs_.push_back(mapvert.Lookup(jvalVerts[0].asInt()));
            llVtxPtrs_.push_back(mapvert.Lookup(jvalVerts[1].asInt()));
            break;
        case 1:
            llVtxPtrs_.push_back(mapvert.Lookup(jvalVerts[0].asInt()));
            break;
        default:
            throw exception("Expected exactly 1 or 2 entries");
        }
    }

Edge::Edge(Vertex::ID id, Json::Value jvalEdge, VertexMap mapvert)
    {
    id_ = idOwner_ = id;
    ReadVerts(jvalEdge["Vertices"], mapvert);
    }

EdgeList::EdgeList(Json::Value& jval, VertexMap mapvert)
    {
    for (Json::ValueIterator itr = jval.begin(); itr != jval.end(); itr++)
        {
        this->push_back(Edge::ReadEdge(itr, mapvert));
        }
    }

string Edge::ToStr() const
    {
    ostringstream ssOut;
    ssOut << "Edge: " << getVtx0().getID();
    if (llVtxPtrs_.size() > 1)
        ssOut << ' ' << getVtx1().getID();
    return ssOut.str();
    }

double Edge::TimeCut() const
    { 
    Position posDiff = getVtx0() - getVtx1();
    double result = posDiff.Magn() / Profile::getCutSpeed();
    return result;
    }

void Edge::ProcLimits(double& xMin, double& xMax, double& yMin, double& yMax, Xform2D xf) const
    {
    xf.Apply(getVtx0()).ProcLimits(xMin, xMax, yMin, yMax);
    xf.Apply(getVtx1()).ProcLimits(xMin, xMax, yMin, yMax);
    // Note: most vertices will be processed twice, that's just not a serious cost
    }

///////////////////////////////////////////////////////////////////

EdgeArc::EdgeArc(Vertex::ID id, Json::Value jvalEdge, VertexMap mapvert)
    : Edge(id)
    {
    id_ = idOwner_ = id;
    ReadVerts(jvalEdge["Vertices"], mapvert);

    posCenter_ = Position(jvalEdge["Center"]);
    Vertex::ID idCwFrom = jvalEdge["ClockwiseFrom"].asInt();

    if (idCwFrom == getVtx0().getID())
        bReverse_ = false;
    else if (idCwFrom == getVtx1().getID())
        bReverse_ = true;
    else
        throw exception("Arc must start from first or second vertex");
    }

double EdgeArc::TimeCut() const
    {
    double fArcLen;
    double radius = (posCenter_ - getVtx0()).Magn();

    if (IsCircle())
        fArcLen = M_PI * radius * radius;
    else
        {
        Position posDiff = getVtx0() - getVtx1();
        double chord = posDiff.Magn();
        fArcLen = 2 * radius * asin(chord / (2 * radius));
        }

    double speed = Profile::getCutSpeed() * exp(-1 / radius);
    return fArcLen / speed;
    }

list<Position> EdgeArc::getChordPts() const
    { // returns a list of evenly spaced points along the arc.  
    // The points are close enought together to assure accuracy within Profile::getAccuracy()
    // The bigger question may be why such a primitive approach?
    //      - The arcs tested so far are nicely oriented with the X/Y axes, but that may not always be the case.
    //      - It's tough to break

    if (llChordPts_.empty())
        {
        Position posStartVector;
        Position posEndVector;
        double fAngle;

        if (IsCircle())
            { // complete circle with just one vertex.
            posStartVector = getVtx0() - posCenter_;
            fAngle = 2 * M_PI; // 360 degr
            }
        else
            {
            posStartVector = (bReverse_ ? getVtx1() : getVtx0()) - posCenter_;
            posEndVector = (bReverse_ ? getVtx0() : getVtx1()) - posCenter_;

            Position posXaxis(1, 0);
            double fAngleStart = posXaxis.ComputeAngle(posStartVector);
            double fAngleEnd = posXaxis.ComputeAngle(posEndVector);
            fAngle = -(fAngleStart - fAngleEnd); // The angle swept by the arc
            double fRadius = posStartVector.Magn();
            }

        double fRadius = posStartVector.Magn();
        double fAngleIncr = 2 * acos(1 - Profile::getAccuracy() / fRadius);
        int nIncrements = 1 + abs (static_cast<int>(fAngle / fAngleIncr)); // nIncrements is the number of chords that will approximate the arc.  That appoximation is only for the purpose of computing plate size via ProcLimits(..)
        fAngleIncr = fAngle / nIncrements;

        /*
        For the block that follows we iterate on a rotation transform
        instead of just iterating an angle and taking sin/cos each time.  This way
        is more efficient but it isn't necessary here.
        */
        Xform2D xf;
        xf.Translate(-posCenter_.getX(), -posCenter_.getY());
        xf.Rotate(fAngleIncr);
        xf.Translate(posCenter_.getX(), posCenter_.getY());
        Position pos = posCenter_ + posStartVector;  // corresponds to the ClockwiseFrom vertex
        llChordPts_.push_back(pos);
        for (int nx = 1; nx < nIncrements - 1; nx++)
            {
            pos = xf.Apply(pos);
            llChordPts_.push_back(pos);
            }
        llChordPts_.push_back(posCenter_ + posEndVector); // assures no accumulated floating point error for the ending vertex
        // Plethora: Floating point error doesn't 't matter for estimating, just a habit from solid modelling
        }
    return llChordPts_;
    }

/*
This implementation of ProcLimits(..) is the entire reason for processing limits by a traversal of all the edges.
For any other edge (i.e. LineSegment), a traversal of all the vertices is all that is needed.
This implementation traverses the path of the arc, evalution points that lie on the arc but are not vertices.
*/
void EdgeArc::ProcLimits(double& xMin, double& xMax, double& yMin, double& yMax, Xform2D xf) const
    {
    for (Position posIter : getChordPts())
        {
        xf.Apply(posIter).ProcLimits(xMin, xMax, yMin, yMax);
        }
    }





