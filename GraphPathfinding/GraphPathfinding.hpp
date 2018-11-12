#pragma once
#include "GraphAdapter.hpp"
#include <functional>
#include <vector>
#include <unordered_map>

class GraphPathfinding
{
public:
    static bool Dijkstra (GraphAdapter *graph, int beginVertex, int endVertex,
            float &outputDistance, std::vector <int> &outputPath);
    static bool AStar (GraphAdapter *graph, int beginVertex, int endVertex,
            float &outputDistance, std::vector <int> &outputPath);
    static bool FordBellman (GraphAdapter *graph, int beginVertex, int endVertex,
            float &outputDistance, std::vector <int> &outputPath);
    static std::unordered_map <int, std::pair <float, int> > FordBellmanScan (GraphAdapter *graph, int beginVertex);
};
