#pragma once
#include "GraphAdapter.hpp"
#include <functional>
#include <vector>

class GraphPathfinding
{
public:
    static bool Dijkstra (GraphAdapter *graph, int beginVertex, int endVertex,
            float &outputDistance, std::vector <int> &outputPath);
    static bool AStar (GraphAdapter *graph, int beginVertex, int endVertex,
            float &outputDistance, std::vector <int> &outputPath);
};
