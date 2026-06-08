/** main.cpp - Application of Graph Data Structure and Algorithms
 *  Lab 13.2: Prim-Jarnik Minimum Spanning Tree on Simplified_USA 7 graph
 */
#include "Graph.h"
#include "GraphAlgorithms.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

/* ------------------------------------------------------------------ */
/*  Helper print functions                                              */
/* ------------------------------------------------------------------ */
void printPathResult(const Graph& graph, const PathResult& result);
void printMSTResult(const Graph& graph, const MSTResult& result);
void printAllPairsShortestDistance(const Graph& graph);

void printPathResult(const Graph& graph, const PathResult& result) {
    if (!result.found) {
        cout << "No path found.\n";
        return;
    }
    printPath(cout, graph, result.path);
    cout << "  / cost = " << result.distance << "\n";
}

void printMSTResult(const Graph& graph, const MSTResult& result) {
    if (!result.connected) {
        cout << "Graph is not connected. MST not found.\n";
        return;
    }
    cout << "Selected edges:\n";
    for (const Edge& e : result.edges) {
        cout << "  Edge(" << setw(4) << graph.vertex(e.from).name
             << " -> "    << setw(4) << graph.vertex(e.to).name
             << " : "     << setw(6) << e.weight << ")\n";
    }
    cout << "Total weight = " << result.totalWeight << "\n";
}

void printAllPairsShortestDistance(const Graph& graph) {
    auto dist = GraphAlgorithms::floydWarshall(graph);
    int  n    = graph.vertexCount();
    cout << "\nAll-pairs shortest distance table by Floyd-Warshall\n";
    cout << "     |";
    for (const Vertex& v : graph.vertices()) cout << setw(7) << v.name;
    cout << "\n------+" << string(n * 7, '-') << "\n";
    for (int i = 0; i < n; ++i) {
        cout << setw(5) << graph.vertex(i).name << " |";
        for (int j = 0; j < n; ++j) {
            if (dist[i][j] == INF) cout << setw(7) << "+oo";
            else                   cout << setw(7) << dist[i][j];
        }
        cout << "\n";
    }
}

/* ------------------------------------------------------------------ */
/*  main                                                                */
/* ------------------------------------------------------------------ */
int main() {
    /** main - Application of Graph Data Structure and Algorithms (1) */
    vector<string> graphFiles = {
        "",
        "Mesh_2x3.txt",
        "Mesh_4x4.txt",
        "Maze_5x5.txt",
        "USA_7_cities.txt",
        "Korea_11.txt"
    };

    while (true) {
        cout << "\nInput graph file\n"
             << " 1: Mesh_2x3.txt\n"
             << " 2: Mesh_4x4.txt\n"
             << " 3: Maze_5x5.txt\n"
             << " 4: USA_7_cities.txt\n"
             << " 5: Korea_11.txt\n"
             << " 0: quit\n"
             << "Choice: ";

        int graphNo = 0;
        cin >> graphNo;
        if (graphNo == 0) break;
        if (graphNo < 0 || graphNo >= static_cast<int>(graphFiles.size())) {
            cout << "Invalid graph file number.\n";
            continue;
        }

        /** main - Application of Graph Data Structure and Algorithms (2) */
        Graph graph;
        try {
            graph = Graph::fgetGraph(graphFiles[graphNo], false);
        } catch (const exception& ex) {
            cout << ex.what() << '\n';
            continue;
        }

        graph.printAdjacencyList(cout);
        graph.printAdjacencyMatrix(cout);

        /* Tests of Graph Algorithms */
        while (true) {
            cout << "\nInput test mode\n"
                 << " 1: DFS path search\n"
                 << " 2: BFS shortest path for unweighted graph\n"
                 << " 3: Dijkstra shortest path for weighted graph\n"
                 << " 4: Floyd-Warshall all-pairs shortest distance\n"
                 << " 5: Prim MST\n"
                 << " 6: Kruskal MST\n"
                 << " 0: select another graph / quit this graph\n"
                 << "Choice: ";

            int mode = 0;
            cin >> mode;
            if (mode == 0) break;

            string startName, targetName;

            switch (mode) {
            /* ---- 1: DFS ---- */
            case 1: {
                cout << "Input start and target vertex names: ";
                cin >> startName >> targetName;
                PathResult result =
                    GraphAlgorithms::dfsPath(graph, startName, targetName);
                cout << "DFS path: ";
                printPathResult(graph, result);
                break;
            }
            /* ---- 2: BFS ---- */
            case 2: {
                cout << "Input start and target vertex names: ";
                cin >> startName >> targetName;
                PathResult result =
                    GraphAlgorithms::bfsShortestPath(graph, startName, targetName);
                cout << "BFS shortest path: ";
                printPathResult(graph, result);
                break;
            }
            /* ---- 3: Dijkstra ---- */
            case 3: {
                cout << "Input start and target vertex names: ";
                cin >> startName >> targetName;
                PathResult result =
                    GraphAlgorithms::dijkstraShortestPath(graph, startName, targetName);
                cout << "Dijkstra shortest path: ";
                printPathResult(graph, result);
                break;
            }
            /* ---- 4: Floyd-Warshall ---- */
            case 4: {
                printAllPairsShortestDistance(graph);
                break;
            }
            /* ---- 5: Prim-Jarnik MST ---- */
            case 5: {
                cout << "Input start vertex name for Prim MST: ";
                cin >> startName;
                cout << "\nTesting Prim-JarnikMST() with start vertex ("
                     << startName << ")\n";
                MSTResult result =
                    GraphAlgorithms::primMST(graph, startName);
                cout << "\nEnd of finding Minimum Spanning Tree by Prim-Jarnik(...):\n";
                printMSTResult(graph, result);
                break;
            }
            /* ---- 6: Kruskal MST ---- */
            case 6: {
                cout << "\nTesting KruskalMST()\n";
                MSTResult result = GraphAlgorithms::kruskalMST(graph);
                cout << "\nEnd of finding Minimum Spanning Tree by Kruskal(...):\n";
                printMSTResult(graph, result);
                break;
            }
            default:
                cout << "Invalid mode.\n";
                break;
            }
        }
    }   // end of while

    return 0;
}
