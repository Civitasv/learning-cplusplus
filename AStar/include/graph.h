#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class RenderWindow;

class Point {
 public:
  Point() : x(0), y(0) {}
  Point(int x, int y) : x(x), y(y) {}

  int x;
  int y;
};

class Node {
 public:
  Node() {}
  Node(int id, Point p) : id(id), p(p), status(0) {}

  int id;
  // 0: 未处理 1: 处理中 2: 完成处理 3: 处于结果路径
  int status;
  Point p;
};

class Edge {
 public:
  Edge() {}
  Edge(int id, int from_node_id, int to_node_id, int cost)
      : id(id),
        from_node_id(from_node_id),
        to_node_id(to_node_id),
        cost(cost) {}

  int id;
  int from_node_id;
  int to_node_id;

  int cost;
};

class Graph {
 private:
  struct NodeCompare {
    bool operator()(const Node* lhs, const Node* rhs) const {
      if (lhs != nullptr && rhs != nullptr) {
        return lhs->id < rhs->id;
      }
      return true;
    }
  };
  struct EdgeCompare {
    bool operator()(const Edge* lhs, const Edge* rhs) const {
      if (lhs != nullptr && rhs != nullptr) {
        return lhs->id < rhs->id;
      }
      return true;
    }
  };

  struct Data {
    std::map<int, Node*> node_dict;
    std::map<Node*, std::vector<Edge*>, NodeCompare> edges_per_node;
    std::map<Edge*, std::pair<Node*, Node*>, EdgeCompare> nodes_per_edge;
  };

 public:
  Graph() : nodes({}), edges({}) {}
  Graph(std::vector<Node> nodes, std::vector<Edge> edges)
      : nodes(nodes), edges(edges) {}

  std::vector<Node> nodes;
  std::vector<Edge> edges;

  int Heuristic(Node* lhs, Node* rhs) {
    return abs(lhs->p.x - rhs->p.x) + abs(lhs->p.y - rhs->p.y);
  }

  void PrepareData(Data* data) {
    for (auto& node : nodes) {
      data->node_dict[node.id] = &node;
    }

    for (auto& edge : edges) {
      Node* from = data->node_dict[edge.from_node_id];
      Node* to = data->node_dict[edge.to_node_id];

      data->edges_per_node[from].push_back(&edge);

      data->nodes_per_edge[&edge] = {from, to};
    }
  }

  void BFS(Node* start, Node* end, RenderWindow* window, int size);

  void Dijkstra(Node* start, Node* end, RenderWindow* window, int size);

  void AStar(Node* start, Node* end, RenderWindow* window, int size);
};