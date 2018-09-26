#include "stdafx.h"

#include "json/json.h"

#include "Position.h"
#include "Vertices.h"
#include "Profile.h"


using namespace std;


Vertex::Vertex(Json::ValueIterator& jiter)
    {
    id_ = Profile::ReadID(jiter); // Profile::IDfromString(jiter.key().asString());

    Json::Value jval = *jiter;
    ProcPosition(jval["Position"]);
    }

void VertexMap::Read(Json::Value& jval)
    {
    for (Json::ValueIterator itr = jval.begin(); itr != jval.end(); itr++)
        {
        VtxPtr sp_vtx = make_shared<Vertex>(itr);
        Add(sp_vtx);
        }
    }

void VertexMap::Add(const VtxPtr& sp_vtx)
    {
    map_.insert(pair<Vertex::ID, VtxPtr>(sp_vtx->getID(), sp_vtx));
    }

string Vertex::ToStr() const
    {
    ostringstream ssOut;
    ssOut << "Vertex: " << getID() << ' ' << Position::ToStr();
    return ssOut.str();
    }




