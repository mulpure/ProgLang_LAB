/** GraphAlgorithms.cpp */
#include "GraphAlgorithms.h"
#include <algorithm>   // for std::reverse()

/** GraphAlgorithms.cpp (1) - buildPath */
std::vector<int>
GraphAlgorithms::buildPath(int start, int target,
                            const std::vector<int>& previous)
{
    std::vector<int> path;
    if (target < 0 || target >= static_cast<int>(previous.size()))
        return path;
    if (previous[target] == -1 && start != target)
        return path;

    for (int cur = target; cur != -1; cur = previous[cur]) {
        path.push_back(cur);
        if (cur == start) break;
    }
    if (path.back() != start)
        return {};   // return empty std::vector<int> {}
    std::reverse(path.begin(), path.end());
    return path;
}

/** GraphAlgorithms.cpp (2) - dfsPath */
PathResult GraphAlgorithms::dfsPath(const Graph& graph,
                                     const std::string& startName,
                                     const std::string& targetName)
{
    int start  = graph.idOf(startName);
    int target = graph.idOf(targetName);
    int n      = graph.vertexCount();

    std::vector<bool> visited(n, false);
    std::vector<int>  previous(n, -1);

    std::function<bool(int)> dfs = [&](int u) -> bool   // lambda function dfs()
    {
        visited[u] = true;
        if (u == target) return true;
        for (const Edge& e : graph.adjacentEdges(u)) {
            int v = e.to;
            if (!visited[v]) {
                previous[v] = u;
                if (dfs(v)) return true;   // reached to target
            }
        }
        return false;
    };   // end of lambda function dfs()

    PathResult result;
    result.found = dfs(start);
    if (result.found) {   // if reached to target
        result.path     = buildPath(start, target, previous);
        result.distance = static_cast<double>(result.path.size() - 1);
    }
    return result;   // return result as PathResult
}

/** GraphAlgorithms.cpp (3) - bfsShortestPath */
PathResult GraphAlgorithms::bfsShortestPath(const Graph& graph,
                                              const std::string& startName,
                                              const std::string& targetName)
{
    int start  = graph.idOf(startName);
    int target = graph.idOf(targetName);
    int n      = graph.vertexCount();

    std::queue<int>   q;
    std::vector<bool> visited(n, false);
    std::vector<int>  previous(n, -1);
    std::vector<int>  level(n, -1);

    visited[start] = true;
    level[start]   = 0;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == target) break;
        for (const Edge& e : graph.adjacentEdges(u)) {
            int v = e.to;
            if (!visited[v]) {
                visited[v]   = true;
                previous[v]  = u;
                level[v]     = level[u] + 1;
                q.push(v);
            }
        }
    }

    /** GraphAlgorithms.cpp (4) */
    PathResult result;
    result.found = visited[target];
    if (result.found) {
        result.path     = buildPath(start, target, previous);
        result.distance = level[target];
    }
    return result;
}

/** GraphAlgorithms.cpp (5) - dijkstraShortestPath */
PathResult GraphAlgorithms::dijkstraShortestPath(const Graph& graph,
                                                   const std::string& startName,
                                                   const std::string& targetName)
{
    int start  = graph.idOf(startName);
    int target = graph.idOf(targetName);
    int n      = graph.vertexCount();

    using Node = std::pair<double, int>;   // distance, vertex id
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    std::vector<double> dist(n, INF);
    std::vector<int>    previous(n, -1);

    dist[start] = 0.0;            // distance of start node is 0.0
    pq.push({0.0, start});        // push pair {0.0, start} into priority_queue
    /* Note: STL priority_queue is providing automatic heap-sort
             using greater<> function, configuring MinHeap */

    while (!pq.empty()) {
        auto [curDist, u] = pq.top();   // 구조적 바인딩 사용, copy 반환
        pq.pop();
        if (curDist > dist[u]) continue;
        if (u == target)       break;

        /** GraphAlgorithms.cpp (6) */
        for (const Edge& e : graph.adjacentEdges(u)) {
            if (e.weight < 0) {
                throw std::logic_error(
                    "Dijkstra cannot be used with negative edge weights.");
            }
            int v = e.to;   // e: u -> v edge

            /* edge relaxation */
            double nextDist = dist[u] + e.weight;
            if (nextDist < dist[v]) {
                dist[v]      = nextDist;
                previous[v]  = u;
                pq.push({nextDist, v});
            }
        }
    }   // end of while

    PathResult result;
    result.found    = dist[target] != INF;
    result.distance = dist[target];
    if (result.found)
        result.path = buildPath(start, target, previous);
    return result;
}

/** Floyd-Warshall all-pairs shortest distance */
std::vector<std::vector<double>>
GraphAlgorithms::floydWarshall(const Graph& graph)
{
    auto dist = graph.adjacencyMatrix();
    int  n    = graph.vertexCount();

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < n; ++j) {
                if (dist[k][j] == INF) continue;
                dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    return dist;
}

/** Prim-Jarnik MST Algorithm */
MSTResult GraphAlgorithms::primMST(const Graph& graph,
                                    const std::string& startName)
{
    if (graph.isDirected()) {
        throw std::logic_error("MST is normally defined for undirected graphs.");
    }

    int start = graph.idOf(startName);
    int n     = graph.vertexCount();
    std::vector<bool> selected(n, false);

    struct Candidate {
        double weight;
        Edge   edge;
        bool operator>(const Candidate& other) const {
            return weight > other.weight;
        }
    };

    std::priority_queue<Candidate, std::vector<Candidate>,
                        std::greater<Candidate>> pq;   // MinHeap

    auto pushEdges = [&](int u)   // lambda function
    {
        selected[u] = true;
        for (const Edge& e : graph.adjacentEdges(u)) {
            if (!selected[e.to])
                pq.push(Candidate{e.weight, e});
        }
    };

    MSTResult result;
    pushEdges(start);

    while (!pq.empty() &&
           static_cast<int>(result.edges.size()) < n - 1) {
        Candidate cur = pq.top();   // select the candidate edge with least weight
        pq.pop();
        if (selected[cur.edge.to]) continue;   // if already selected, then skip it

        result.edges.push_back(cur.edge);
        result.totalWeight += cur.edge.weight;
        pushEdges(cur.edge.to);
    }

    result.connected =
        static_cast<int>(result.edges.size()) == n - 1;
    return result;
}

/** DisjointSet (for Kruskal's MST) */
class DisjointSet {
private:
    std::vector<int> parent_;   // parent of vertex
    std::vector<int> rank_;     // sub_MST의 높이

public:
    explicit DisjointSet(int n) : parent_(n), rank_(n, 0)
    {
        std::iota(parent_.begin(), parent_.end(), 0);
        // iota() : 연속적인 값을 컨테이너에 자동적으로 채움
        // parent_[0]=0; parent_[1]=1; ... ; parent_[n-1]=n-1;
    }

    int find(int x) {
        if (parent_[x] != x)
            parent_[x] = find(parent_[x]);   // path compression
        return parent_[x];
    }

    bool unite(int a, int b) {
        // unite set of a and set of b into one merged set
        int rootA = find(a);
        int rootB = find(b);
        if (rootA == rootB) return false;   // already in same set
        if (rank_[rootA] < rank_[rootB]) std::swap(rootA, rootB);
        parent_[rootB] = rootA;
        if (rank_[rootA] == rank_[rootB]) ++rank_[rootA];
        return true;
    }
};

/** Kruskal MST Algorithm */
MSTResult GraphAlgorithms::kruskalMST(const Graph& graph) {
    if (graph.isDirected()) {
        throw std::logic_error("MST is normally defined for undirected graphs.");
    }

    std::vector<Edge> edges = graph.edges(true);
    std::sort(edges.begin(), edges.end(),
        [](const Edge& a, const Edge& b) {   // Lambda function
            return a.weight < b.weight;       // condition of order a and b
        }
    );   // sort edges in ascending order of weight

    DisjointSet ds(graph.vertexCount());
    MSTResult   result;

    for (const Edge& e : edges) {
        if (ds.unite(e.from, e.to)) {
            // unite set of e.from and set of e.to
            // if set of e.from and set of e.to are already same,
            // skip this part to avoid cycle
            result.edges.push_back(e);
            result.totalWeight += e.weight;
            if (static_cast<int>(result.edges.size()) ==
                graph.vertexCount() - 1)
                break;
        }
    }

    result.connected =
        static_cast<int>(result.edges.size()) == graph.vertexCount() - 1;
    return result;
}
