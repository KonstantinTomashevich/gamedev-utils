#include "GraphPathfinding.hpp"
#include <unordered_map>
#include <queue>

typedef std::pair <int, std::pair <int, float> > QueueData; // vertex : (comeFromVertex, cost)
class GreaterComparator
{
public:
    bool operator () (const QueueData &first, const QueueData &second) const
    {
        return first.second.second > second.second.second;
    }
};

bool GraphPathfinding::Dijkstra (GraphAdapter *graph, int beginVertex, int endVertex, float &outputDistance,
        std::vector <int> &outputPath)
{
    if (beginVertex == endVertex)
    {
        outputDistance = 0.0f;
        outputPath.push_back (beginVertex);
        return true;
    }

    std::unordered_map <int, int> comeFrom;
    std::priority_queue <QueueData, std::vector <QueueData>, GreaterComparator> queue;
    queue.push (std::make_pair (beginVertex, std::make_pair (beginVertex, 0.0f)));

    while (!queue.empty ())
    {
        int vertex = queue.top ().first;
        int comeFromVertex = queue.top ().second.first;
        float cost = queue.top ().second.second;
        queue.pop ();

        if (vertex == endVertex)
        {
            std::vector <int> reversedPath;
            reversedPath.push_back (vertex);
            auto comeFromIterator = comeFrom.find (comeFromVertex);

            while (comeFromIterator->first != beginVertex)
            {
                reversedPath.push_back (comeFromIterator->first);
                comeFromIterator = comeFrom.find (comeFromIterator->second);
            }

            reversedPath.push_back (beginVertex);
            unsigned int length = reversedPath.size ();
            outputPath.resize (length);

            for (unsigned int index = 0; index < length; ++index)
            {
                outputPath [length - index - 1] = reversedPath [index];
            }

            outputDistance = cost;
            return true;
        }

        auto comeFromIterator = comeFrom.find (vertex);
        if (comeFromIterator == comeFrom.end ())
        {
            comeFrom [vertex] = comeFromVertex;
            SimpleIterator <VertexOutcomingConnection> *neighborsIterator = graph->GetOutcomingConnections (vertex);

            while (neighborsIterator->Valid ())
            {
                VertexOutcomingConnection connection = neighborsIterator->Get ();
                neighborsIterator->Increment ();
                queue.push (std::make_pair (connection.target, std::make_pair (vertex, cost + connection.weight)));
            }

            delete neighborsIterator;
        }
    }

    return false;
}

bool GraphPathfinding::AStar (GraphAdapter *graph, int beginVertex, int endVertex, float &outputDistance,
        std::vector <int> &outputPath)
{
    if (beginVertex == endVertex)
    {
        outputDistance = 0.0f;
        outputPath.push_back (beginVertex);
        return true;
    }

    std::unordered_map <int, std::pair <int, float> > comeFrom; // vertex : (comeFromVertex, cost)
    std::priority_queue <QueueData, std::vector <QueueData>, GreaterComparator> queue;

    queue.push (std::make_pair (beginVertex, std::make_pair (beginVertex, 0.0f)));
    comeFrom [beginVertex].first = beginVertex;
    comeFrom [beginVertex].second = 0.0f;

    while (!queue.empty ())
    {
        int vertex = queue.top ().first;
        int comeFromVertex = queue.top ().second.first;
        float priority = queue.top ().second.second;
        queue.pop ();

        if (vertex == endVertex)
        {
            std::vector <int> reversedPath;
            auto comeFromIterator = comeFrom.find (vertex);
            outputDistance = comeFromIterator->second.second;

            while (comeFromIterator->first != beginVertex)
            {
                reversedPath.push_back (comeFromIterator->first);
                comeFromIterator = comeFrom.find (comeFromIterator->second.first);
            }

            reversedPath.push_back (beginVertex);
            unsigned int length = reversedPath.size ();
            outputPath.resize (length);

            for (unsigned int index = 0; index < length; ++index)
            {
                outputPath [length - index - 1] = reversedPath [index];
            }

            return true;
        }

        auto comeFromIterator = comeFrom.find (vertex);
        if (comeFromIterator->second.first == comeFromVertex)
        {
            float cost = comeFromIterator->second.second;
            SimpleIterator <VertexOutcomingConnection> *neighborsIterator = graph->GetOutcomingConnections (vertex);

            while (neighborsIterator->Valid ())
            {
                VertexOutcomingConnection connection = neighborsIterator->Get ();
                neighborsIterator->Increment ();

                auto neighborComeFromIterator = comeFrom.find (connection.target);
                if (neighborComeFromIterator == comeFrom.end () ||
                        neighborComeFromIterator->second.second > cost + connection.weight)
                {
                    comeFrom [connection.target].first = vertex;
                    comeFrom [connection.target].second = cost + connection.weight;

                    queue.push (std::make_pair (connection.target, std::make_pair (
                            vertex, graph->HeuristicDistance (connection.target, endVertex))));
                }
            }

            delete neighborsIterator;
        }
    }

    return false;
}
