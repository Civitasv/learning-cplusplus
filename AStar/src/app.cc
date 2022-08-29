#include "include/app.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <memory>
#include <random>

#include "include/graph.h"
#include "include/render_window.h"

App::App()
    : running(true),
      pause(false),
      row(10),
      col(10),
      size(80),
      start(0),
      end(98) {}

bool App::OnInitialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "HEY.. SDL_Init FAILED: " << SDL_GetError() << std::endl;
    return false;
  }
  if (!(IMG_Init(IMG_INIT_PNG))) {
    std::cout << "HEY.. IMG_Init FAILED: " << SDL_GetError() << std::endl;
    return false;
  }

  window = {"A* Algrorithm", 1920, 1080, 0, 0};

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
      if (i < row - 1) {
        Node& down_node = nodes[(i + 1) * row + j];
        Edge edge = {index++, node.id, down_node.id, 1};
        edges.push_back(edge);
      }
    }
  }
  graph = {nodes, edges};

  return true;
}

void App::OnEvent(SDL_Event* event) {
  if (pause && event->key.keysym.sym != SDLK_r) {
    return;
  }
  if (event->type == SDL_QUIT) {
    running = false;
  } else if (event->type == SDL_KEYDOWN) {
    SDL_Keycode code = event->key.keysym.sym;
    if (code == SDLK_LEFT) {
    }  // left arrow
    else if (code == SDLK_RIGHT) {
    }  // right arrow
    else if (code == SDLK_UP) {
    }  // up arrow
    else if (code == SDLK_DOWN) {
    }  // down arrow
    else if (code == SDLK_q) {
      running = false;
    }  // q for quit
    else if (code == SDLK_r) {
      pause = !pause;
    }  // r for pause and resume
  }
}

void App::OnLoop() {
  if (pause) {
    return;
  }
  for (Node& node : graph.nodes) {
    node.status = 0;
  }
}

void App::OnRender() {
  if (pause) {
    return;
  }
  graph.BFS(&graph.nodes[start], &graph.nodes[end], &window, size);
}

void App::OnCleanUp() {
  window.CleanUp();
  SDL_Quit();
}

int App::Run() {
  // 1. Initialize
  if (!OnInitialize()) {
    return -1;
  }

  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      // 2. Event
      OnEvent(&event);
    }
    // 3. Loop, update data according to event
    OnLoop();
    // 4. Render
    OnRender();

    SDL_Delay(2000);
  }

  // 5. Cleanup
  OnCleanUp();
  return 0;
}