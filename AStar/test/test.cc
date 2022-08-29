#include <iostream>
#include <vector>

#include "include/graph.h"

void TestGraph();

int main(int argc, const char** argv) { TestGraph(); }

void TestGraph() {
  int row = 10, col = 10;
  std::vector<Node> nodes;
  int index = 0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      // make a node
      Node node = {index++, {i, j}};
      nodes.push_back(node);
    }
  }

  std::vector<Edge> edges;
  index = 0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      Node& node = nodes[i * row + j];
      // left
      if (j > 0) {
        Node& left_node = nodes[i * row + j - 1];
        Edge edge = {index++, node.id, left_node.id, 1};
        edges.push_back(edge);
      }
      // right
      if (j < col - 1) {
        Node& right_node = nodes[i * row + j + 1];
        Edge edge = {index++, node.id, right_node.id, 1};
        edges.push_back(edge);
      }
      // top
      if (i > 0) {
        Node& top_node = nodes[(i - 1) * row + j];
        Edge edge = {index++, node.id, top_node.id, 1};
        edges.push_back(edge);
      }
      // down
      if (j < row - 1) {
        Node& down_node = nodes[(i + 1) * row + j];
        Edge edge = {index++, node.id, down_node.id, 1};
        edges.push_back(edge);
      }
    }
  }
  Graph graph = {nodes, edges};

  for (auto& node : graph.nodes) {
    std::cout << node.status << ' ';
  }
}