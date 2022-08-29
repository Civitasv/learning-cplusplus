#include "include/graph.h"

#include "include/render_window.h"

void Graph::BFS(Node* start, Node* end, RenderWindow* window, int size) {
  Data data;
  PrepareData(&data);

  auto frontier = std::queue<Node*>();
  frontier.push(start);

  auto came_from = std::map<Node*, Node*, NodeCompare>();
  came_from[start] = start;

  while (!frontier.empty()) {
    Node* current = frontier.front();
    current->status = 2;
    frontier.pop();

    if (current->id == end->id) {
      while (!frontier.empty()) {
        Node* item = frontier.front();
        item->status = 2;
        frontier.pop();
      }
      break;
    }
    for (auto edge : data.edges_per_node[current]) {
      Node* node = data.node_dict[edge->to_node_id];
      if (came_from.find(node) == came_from.end()) {
        node->status = 1;  // 处理中
        frontier.push(node);
        came_from[node] = current;
      }
    }
    window->Render(*this, size);
    SDL_Delay(500);
  }

  // find the path
  Node* current = end;
  while (current->id != start->id) {
    current->status = 3;
    current = came_from[current];
  }

  start->status = 3;
  window->Render(*this, size);
}

void Graph::Dijkstra(Node* start, Node* end, RenderWindow* window, int size) {
  Data data;
  PrepareData(&data);

  typedef struct NodeVal {
    Node* node;
    int cost;
  } NodeVal;

  auto cmp = [](NodeVal a, NodeVal b) { return a.cost > b.cost; };

  // 每次选取最小的，相当于集合 U
  std::priority_queue<NodeVal, std::vector<NodeVal>, decltype(cmp)> frontier(
      cmp);
  frontier.push({start, 0});

  auto came_from = std::map<Node*, Node*, NodeCompare>();
  // 记录 start 到其他 node 的值，keys 相当于集合 S
  auto cost_so_far = std::map<Node*, int, NodeCompare>();

  came_from[start] = start;
  cost_so_far[start] = 0;

  while (!frontier.empty()) {
    auto current = frontier.top();
    current.node->status = 2;
    frontier.pop();

    if (current.node->id == end->id) {
      while (!frontier.empty()) {
        NodeVal item = frontier.top();
        item.node->status = 2;
        frontier.pop();
      }
      break;
    }

    for (auto edge : data.edges_per_node[current.node]) {
      Node* node = data.node_dict[edge->to_node_id];
      int new_cost = cost_so_far[current.node] + edge->cost;
      if (cost_so_far.find(node) == cost_so_far.end() ||
          new_cost < cost_so_far[node]) {
        node->status = 1;  // 处理中
        cost_so_far[node] = new_cost;

        frontier.push({node, new_cost});

        came_from[node] = current.node;
      }
    }
    window->Render(*this, size);
    SDL_Delay(500);
  }

  // find the path
  Node* current = end;
  while (current->id != start->id) {
    current->status = 3;
    current = came_from[current];
  }

  start->status = 3;
  window->Render(*this, size);
}

void Graph::AStar(Node* start, Node* end, RenderWindow* window, int size) {
  Data data;
  PrepareData(&data);

  typedef struct NodeVal {
    Node* node;
    int cost;
  } NodeVal;

  auto cmp = [](NodeVal a, NodeVal b) { return a.cost > b.cost; };

  // 每次选取最小的，相当于集合 U
  std::priority_queue<NodeVal, std::vector<NodeVal>, decltype(cmp)> frontier(
      cmp);
  frontier.push({start, 0});

  auto came_from = std::map<Node*, Node*, NodeCompare>();
  // 记录 start 到其他 node 的值，keys 相当于集合 S
  auto cost_so_far = std::map<Node*, int, NodeCompare>();

  came_from[start] = start;
  cost_so_far[start] = 0;

  while (!frontier.empty()) {
    auto current = frontier.top();
    frontier.pop();
    current.node->status = 2;  // 完成处理

    if (current.node->id == end->id) {
      while (!frontier.empty()) {
        NodeVal item = frontier.top();
        item.node->status = 2;
        frontier.pop();
      }
      break;
    }

    for (auto edge : data.edges_per_node[current.node]) {
      Node* node = data.node_dict[edge->to_node_id];
      int new_cost = cost_so_far[current.node] + edge->cost;
      if (cost_so_far.find(node) == cost_so_far.end() ||
          new_cost < cost_so_far[node]) {
        node->status = 1;  // 处理中
        cost_so_far[node] = new_cost;

        frontier.push({node, new_cost + Heuristic(end, node)});

        came_from[node] = current.node;
      }
    }
    window->Render(*this, size);
    SDL_Delay(500);
  }

  // find the path
  Node* current = end;
  while (current->id != start->id) {
    current->status = 3;
    current = came_from[current];
  }

  start->status = 3;
  window->Render(*this, size);
}