/*
 * ============================================================
 *  프로그래밍언어  실습 13.1
 *  Dijkstra 알고리즘을 사용한 최단거리 경로 탐색
 *
 *  컴파일 :
 *    g++ -std=c++17 -O2 -o Lab13_1 \
 *        Graph.cpp GraphAlgorithms.cpp Lab13_1_main.cpp
 *  실행 :
 *    ./Lab13_1          (Korea_11.txt 가 같은 폴더에 있어야 함)
 * ============================================================
 */

#include "Graph.h"
#include "GraphAlgorithms.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

/* ── 출력 헬퍼  (main 에서 선언) ────────────────────────── */
void printPathResult(const Graph& graph, const PathResult& result)
{
    if (result.found) {
        printPath(cout, graph, result.path);
        cout << "  / cost = " << (int)result.distance << "\n";
    }
    else {
        cout << "No path found\n";
    }
}

void printMSTResult(const Graph& graph, const MSTResult& result)
{
    if (!result.connected) { cout << "Graph is not connected.\n"; return; }
    cout << "Selected edges:\n";
    for (const Edge& e : result.edges)
        cout << "  " << setw(4) << graph.vertex(e.from).name
        << " -- " << setw(4) << graph.vertex(e.to).name
        << "  weight=" << (int)e.weight << "\n";
    cout << "Total weight = " << (int)result.totalWeight << "\n";
}

void printAllPairsShortestDistance(const Graph& graph)
{
    auto dist = GraphAlgorithms::floydWarshall(graph);
    int  n = graph.vertexCount();
    cout << "\nAll-pairs shortest distance table by Floyd-Warshall\n";
    cout << "      |";
    for (const Vertex& v : graph.vertices()) cout << setw(6) << v.name;
    cout << "\n------+" << string(n * 6, '-') << '\n';
    for (int i = 0; i < n; ++i) {
        cout << setw(5) << graph.vertex(i).name << " |";
        for (int j = 0; j < n; ++j) {
            if (dist[i][j] == INF) cout << setw(6) << "+oo";
            else                   cout << setw(6) << (int)dist[i][j];
        }
        cout << '\n';
    }
}

/* ============================================================
 *  실습 13.1 핵심 : 단계별 거리표 출력 Dijkstra
 *
 *  슬라이드 p.77 출력 구조 :
 *    round [1] ~ round [n-2] : 선택 → relax → 출력
 *    target 선택 시           : "reached to target node" 출력 후 종료
 *    (target 의 round 행은 출력하지 않음)
 * ============================================================ */
void testDijkstraVerbose(const Graph& graph,
    const string& startName,
    const string& targetName)
{
    int start = graph.idOf(startName);
    int target = graph.idOf(targetName);
    int n = graph.vertexCount();

    vector<double> dist(n, INF);
    vector<int>    previous(n, -1);
    vector<bool>   selected(n, false);

    /* ── 시작 노드 사전 선택 & 초기 relaxation ── */
    dist[start] = 0.0;
    selected[start] = true;
    for (const Edge& e : graph.adjacentEdges(start)) {
        if (e.weight < dist[e.to]) {
            dist[e.to] = e.weight;
            previous[e.to] = start;
        }
    }

    /* ── 테이블 헤더 ── */
    cout << "\nDijkstra::Least Cost from Vertex ("
        << startName << ") at each round :\n";
    cout << "           |";
    for (const Vertex& v : graph.vertices())
        cout << setw(5) << v.name;
    cout << "\n";

    /* ── 메인 루프 ── */
    for (int round = 1; round < n; ++round) {

        /* 미선택 정점 중 dist 최솟값 정점 선택 */
        int u = -1;
        for (int i = 0; i < n; ++i)
            if (!selected[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        if (u == -1 || dist[u] == INF) break;

        selected[u] = true;

        /* ── target 도달 : round 행 대신 reached 메시지 출력 ── */
        if (u == target) {
            cout << "\nreached to target node (" << graph.vertex(u).name
                << ") : Least Cost from " << startName
                << " to " << targetName
                << " = " << (int)dist[u] << "\n";
            break;
        }

        /* Edge Relaxation */
        for (const Edge& e : graph.adjacentEdges(u)) {
            if (!selected[e.to]) {
                double nd = dist[u] + e.weight;
                if (nd < dist[e.to]) {
                    dist[e.to] = nd;
                    previous[e.to] = u;
                }
            }
        }

        /* 라운드 결과 출력 (relaxation 완료 후) */
        cout << "round [" << setw(2) << round << "] |";
        for (int i = 0; i < n; ++i) {
            if (dist[i] == INF) cout << setw(5) << "+oo";
            else                cout << setw(5) << (int)dist[i];
        }
        cout << "  ==> selected vertex : "
            << graph.vertex(u).name << "\n";
    }

    /* ── 경로 재구성 & 출력 ── */
    if (dist[target] == INF) {
        cout << "No path from " << startName << " to " << targetName << "\n";
        return;
    }

    vector<int> path;
    for (int cur = target; cur != -1; cur = previous[cur]) {
        path.push_back(cur);
        if (cur == start) break;
    }
    reverse(path.begin(), path.end());

    cout << "\nPath found by DijkstraShortestPath from "
        << startName << " to " << targetName << " : "
        << "Path found (" << startName << " => " << targetName << ") : ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) cout << " -> ";
        cout << graph.vertex(path[i]).name;
    }
    cout << "\n";
}

/* ============================================================
 *  main  ─ 실습 13.1
 * ============================================================ */
int main()
{
    /* ── 그래프 파일 읽기 ── */
    Graph graph;
    try {
        graph = Graph::fgetGraph("Korea_11.txt", false);
    }
    catch (const exception& ex) {
        cerr << ex.what() << "\n";
        return 1;
    }

    graph.printAdjacencyList(cout);
    graph.printAdjacencyMatrix(cout);

    /* ── 테스트 모드 메뉴 ── */
    while (true) {
        cout << "\nInput test mode"
            << "\n  1: DFS path search"
            << "\n  2: BFS shortest path for unweighted graph"
            << "\n  3: Dijkstra shortest path for weighted graph"
            << "\n  4: Floyd-Warshall all-pairs shortest distance"
            << "\n  5: Prim MST"
            << "\n  6: Kruskal MST"
            << "\n  0: select another graph / quit this graph"
            << "\nChoice: ";

        int mode = 0;
        cin >> mode;
        if (mode == 0) break;

        if (mode == 1) {
            cout << "Input start and target vertex names: ";
            string s, t;  cin >> s >> t;
            PathResult r = GraphAlgorithms::dfsPath(graph, s, t);
            cout << "DFS path: ";
            printPathResult(graph, r);
        }
        else if (mode == 2) {
            cout << "Input start and target vertex names: ";
            string s, t;  cin >> s >> t;
            PathResult r = GraphAlgorithms::bfsShortestPath(graph, s, t);
            cout << "BFS shortest path: ";
            printPathResult(graph, r);
        }
        else if (mode == 3) {
            cout << "\nTesting Dijkstra Shortest Path Finder\n";
            while (true) {
                cout << "Input start and end vertex names of path"
                    " to search shortest path (. to quit) : ";
                string s, t;
                cin >> s;
                if (s == ".") break;
                cin >> t;
                try {
                    testDijkstraVerbose(graph, s, t);
                    cout << "\nDijkstra shortest path: ";
                    PathResult r =
                        GraphAlgorithms::dijkstraShortestPath(graph, s, t);
                    printPathResult(graph, r);
                }
                catch (const exception& ex) {
                    cout << ex.what() << "\n";
                }
            }
        }
        else if (mode == 4) {
            printAllPairsShortestDistance(graph);
        }
        else if (mode == 5) {
            cout << "Input start vertex name for Prim MST: ";
            string s;  cin >> s;
            MSTResult r = GraphAlgorithms::primMST(graph, s);
            printMSTResult(graph, r);
        }
        else if (mode == 6) {
            MSTResult r = GraphAlgorithms::kruskalMST(graph);
            printMSTResult(graph, r);
        }
    }

    return 0;
}