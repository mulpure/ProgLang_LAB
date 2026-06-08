/** Graph.h */
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits>
#include <stdexcept>
#include <algorithm>

constexpr double INF = std::numeric_limits<double>::infinity();

struct Vertex {
    int id{};        // initial default value 0
    std::string name;
};

struct Edge {
    int from{};
    int to{};
    double weight{}; // initial default value 0.0
};

inline std::ostream& operator<<(std::ostream& os, const Vertex& v) {
    return os << v.name;
}

inline std::ostream& operator<<(std::ostream& os, const Edge& e) {
    return os << "Edge(" << e.from << " -> " << e.to
              << ", w=" << e.weight << ")";
}

class Graph {
private:
    std::string _graphName;
    bool _directed{};
    std::vector<Vertex> _vertices;
    std::vector<std::vector<Edge>> _adj;
    std::unordered_map<std::string, int> _idByName;

public:
    // 명시적 생성자 호출, 암시적 생성자 호출 또는 암시적 형 변환 방지
    explicit Graph(std::string name = "", bool directed = false);

    const std::string& name() const noexcept;
    bool isDirected() const noexcept;
    int vertexCount() const noexcept;
    int edgeCount() const noexcept;

    int addVertex(const std::string& vertexName);
    void addEdge(const std::string& fromName, const std::string& toName,
                 double weight = 1.0);
    void addEdge(int from, int to, double weight = 1.0);

    bool hasVertex(const std::string& vertexName) const;
    int idOf(const std::string& vertexName) const;
    const Vertex& vertex(int id) const;
    const std::vector<Vertex>& vertices() const noexcept;
    const std::vector<Edge>& adjacentEdges(int id) const;
    std::vector<Edge> edges(bool uniqueUndirectedEdges = true) const;
    std::vector<std::vector<double>> adjacencyMatrix() const;

    void printAdjacencyList(std::ostream& os) const;
    void printAdjacencyMatrix(std::ostream& os) const;

    static Graph fgetGraph(const std::string& fileName, bool directed = false);
};

std::vector<std::string> namesOfPath(const Graph& g, const std::vector<int>& path);
void printPath(std::ostream& os, const Graph& g, const std::vector<int>& path);

#endif
