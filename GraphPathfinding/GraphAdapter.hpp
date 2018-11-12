#pragma once

#include <vector>

struct VertexOutcomingConnection
{
    int target;
    float weight;
};

struct GraphEdge
{
    int inVertex;
    int outVertex;
    float weight;
};

template <class T>
class SimpleIterator
{
public:
    virtual void Increment () = 0;
    virtual T Get () = 0;
    virtual bool Valid () = 0;
};

class GraphAdapter
{
public:
    virtual SimpleIterator <VertexOutcomingConnection> *GetOutcomingConnections (int vertex) const = 0;
    virtual float HeuristicDistance (int beginVertex, int endVertex) const = 0;
    virtual void GetEdges (std::vector <GraphEdge> &output) const = 0;
};
