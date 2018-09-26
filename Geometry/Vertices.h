#pragma once

#include "json/json.h"

#include "Position.h"

class Vertex : public Position
    {
    public:
        typedef Json::Value::Int ID; // not the best place for this, but not worth creating a constants.h file

    private:
        ID id_;

    public:
        Vertex() {}; // todo: remove later
        Vertex(Json::ValueIterator& jiter);

        ID getID() const { return id_;  }

        std::string ToStr() const;

        static std::string JsonTypeName() { return "Vertex"; }
    };

class VertexMap
    {
    public:
        typedef std::shared_ptr<const Vertex> VtxPtr;
        typedef std::map<Vertex::ID, const VtxPtr>::iterator iterator; // I often use typedef to hide this kind of messy C++

    private:
        std::map<Vertex::ID, const VtxPtr> map_;

    public:

        VertexMap() {}; // todo: remove later
        
        void Read(Json::Value& jval);
        
        void Add(const VtxPtr& sp_vtx);
            
        const VtxPtr Lookup(Vertex::ID id)
            {
            const VtxPtr result = map_[id];
            return result;
            }

        std::list<VtxPtr> getVertices()
            {
            std::list<VtxPtr> result;
            for (VertexMap::iterator vtxiter = map_.begin(); vtxiter != map_.end(); vtxiter++)
                result.push_back(vtxiter->second);
            return result;
            }
      
        static std::string JsonTypeName() { return "Vertices"; }
    };


