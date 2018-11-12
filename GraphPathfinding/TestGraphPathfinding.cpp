#include <GraphPathfinding/GraphAdapter.hpp>
#include <GraphPathfinding/GraphPathfinding.hpp>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <iostream>

class MyGraph : public GraphAdapter
{
public:
    template <class T>
    class MyIterator : public SimpleIterator <T>
    {
    public:
        MyIterator (const std::vector <T> *values) : values_ (values), index_ (0)
        {}

        virtual ~MyIterator () = default;

        virtual void Increment ()
        { ++index_; }

        virtual T Get ()
        { return (*values_)[index_]; }

        virtual bool Valid ()
        { return index_ < values_->size (); }

    protected:
        int index_;
        const std::vector <T> *values_;
    };

    MyGraph () : neighborsLists_ (), virtualPositions_ ()
    {}

    virtual ~MyGraph () = default;
    void AddVertex (int index, const std::vector <VertexOutcomingConnection> &connections,
            const std::pair <float, float> &virtualPosition);

    virtual SimpleIterator <VertexOutcomingConnection> *GetOutcomingConnections (int vertex) const;
    virtual float HeuristicDistance (int beginVertex, int endVertex) const;
    virtual void GetEdges (std::vector <GraphEdge> &output) const;

protected:
    std::unordered_map <int, std::vector <VertexOutcomingConnection> > neighborsLists_;
    std::unordered_map <int, std::pair <float, float> > virtualPositions_;
};

void MyGraph::AddVertex (int index, const std::vector <VertexOutcomingConnection> &connections,
        const std::pair <float, float> &virtualPosition)
{
    neighborsLists_[index] = connections;
    virtualPositions_[index].first = virtualPosition.first;
    virtualPositions_[index].second = virtualPosition.second;
}

SimpleIterator <VertexOutcomingConnection> *MyGraph::GetOutcomingConnections (int vertex) const
{
    return new MyIterator <VertexOutcomingConnection> (&neighborsLists_.at (vertex));
}

float MyGraph::HeuristicDistance (int beginVertex, int endVertex) const
{
    float dX = virtualPositions_.at (beginVertex).first - virtualPositions_.at (endVertex).first;
    float dY = virtualPositions_.at (beginVertex).second - virtualPositions_.at (endVertex).second;
    return std::sqrt (dX * dX + dY * dY);
}

void MyGraph::GetEdges (std::vector <GraphEdge> &output) const
{
    for (auto &vertexNeighbors : neighborsLists_)
    {
        for (auto &outcomingConnection : vertexNeighbors.second)
        {
            output.push_back ({vertexNeighbors.first, outcomingConnection.target, outcomingConnection.weight});
        }
    }
}

int main ()
{
    /*
     * 1 -- 3 -- 5
     * |   /|   /|
     * | /  | /  |
     * 0 -- 2 -- 4
     */
    MyGraph graph;
    graph.AddVertex (0, {{1, 1.5f},
                         {2, 1.0f},
                         {3, 2.5f}}, std::make_pair (0, 0));
    graph.AddVertex (1, {{0, 1.5f},
                         {3, 1.5f}}, std::make_pair (0, 1));
    graph.AddVertex (2, {{0, 1.0f},
                         {4, 1.0f},
                         {3, 1.5f},
                         {5, 2.5f}}, std::make_pair (1, 0));
    graph.AddVertex (3, {{1, 1.5f},
                         {2, 1.5f},
                         {5, 1.5f},
                         {0, 2.5f}}, std::make_pair (1, 1));
    graph.AddVertex (4, {{2, 1.0f},
                         {5, 1.0f}}, std::make_pair (2, 0));
    graph.AddVertex (5, {{3, 1.5f},
                         {2, 2.5f},
                         {4, 1.0f}}, std::make_pair (2, 1));

    std::vector <int> result;
    float distance;
    GraphPathfinding::Dijkstra (&graph, 0, 5, distance, result);

    std::cout << "Dijkstra:" << std::endl;
    std::cout << "    Distance: " << distance << "." << std::endl;
    std::cout << "    Path: ";

    for (unsigned int index = 0; index < result.size (); ++index)
    {
        std::cout << result[index] << (index == result.size () - 1 ? "." : " -> ");
    }
    std::cout << std::endl;

    if (distance < 2.99f || distance > 3.01f)
    {
        std::cout << "    Incorrect distance, expected 3!" << std::endl;
    }

    std::vector <int> expected = {0, 2, 4, 5};
    if (result != expected)
    {
        std::cout << "    Incorrect path, expected 0 -> 2 -> 4 -> 5!" << std::endl;
    }

    GraphPathfinding::AStar (&graph, 0, 5, distance, result);
    std::cout << "AStar:" << std::endl;
    std::cout << "    Distance: " << distance << "." << std::endl;
    std::cout << "    Path: ";

    for (unsigned int index = 0; index < result.size (); ++index)
    {
        std::cout << result[index] << (index == result.size () - 1 ? "." : " -> ");
    }
    std::cout << std::endl;

    if (distance < 3.99f || distance > 4.01f)
    {
        std::cout << "    Incorrect distance, expected 4!" << std::endl;
    }

    expected = {0, 3, 5};
    if (result != expected)
    {
        std::cout << "    Incorrect path, expected 0 -> 3 -> 5!" << std::endl;
    }

    GraphPathfinding::FordBellman (&graph, 0, 5, distance, result);
    std::cout << "Ford-Bellman:" << std::endl;
    std::cout << "    Distance: " << distance << "." << std::endl;
    std::cout << "    Path: ";

    for (unsigned int index = 0; index < result.size (); ++index)
    {
        std::cout << result[index] << (index == result.size () - 1 ? "." : " -> ");
    }
    std::cout << std::endl;

    if (distance < 2.99f || distance > 3.01f)
    {
        std::cout << "    Incorrect distance, expected 3!" << std::endl;
    }

    expected = {0, 2, 4, 5};
    if (result != expected)
    {
        std::cout << "    Incorrect path, expected 0 -> 2 -> 4 -> 5!" << std::endl;
    }

    std::cin.get ();
    return 0;
}
