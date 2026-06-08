/** GraphAlgorithms.h (1) */
#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "Graph.h"
#include <queue>
#include <stack>
#include <functional>
#include <numeric>

struct PathResult {
    bool         found{};        // default initial value = false
    double       distance{INF};
    std::vector<int> path;
};

struct MSTResult {
    bool         connected{};
    double       totalWeight{};  // default initial value = 0.0
    std::vector<Edge> edges;
};

/** GraphAlgorithms.h (2) */
class GraphAlgorithms {
public:
    static PathResult dfsPath(
        const Graph& graph,
        const std::string& startName, const std::string& targetName);

    static PathResult bfsShortestPath(
        const Graph& graph,
        const std::string& startName, const std::string& targetName);

    static PathResult dijkstraShortestPath(
        const Graph& graph,
        const std::string& startName, const std::string& targetName);

    static std::vector<std::vector<double>> floydWarshall(const Graph& graph);

    static MSTResult kruskalMST(const Graph& graph);

    static MSTResult primMST(const Graph& graph,
                              const std::string& startName);

private:
    static std::vector<int> buildPath(int start, int target,
                                       const std::vector<int>& previous);
};

#endif
