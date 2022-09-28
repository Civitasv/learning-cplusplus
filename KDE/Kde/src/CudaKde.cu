#include <device_launch_parameters.h>
#include <thrust/copy.h>
#include <thrust/device_malloc.h>
#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/host_vector.h>
#include <thrust/sort.h>

#include <chrono>
#include <memory>

#include "CudaKde.cuh"
#include "Point.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace kde {
__device__ float GPUDist(const Point& p1, const Point& p2) {
  double a = p1.lon - p2.lon;
  double b = p1.lat - p2.lat;

  return a * a + b * b;
}

__device__ float GPUKernel(float t) {
  float val = (1 - t * t);
  return (3 / M_PI) * val * val;
}

__global__ void CalCulateKDEKernel(const Point* pts, const Point* calculate_pts,
                                   Item* res, float band_width, int width,
                                   int height, int pts_size) {
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  if (index > width * height) return;
  Point p = calculate_pts[index];

  float f_estimate = 0.0f;

  for (int m = 0; m < pts_size; m++) {
    float distance = GPUDist(pts[m], p);
    /*printf("index: %d, x: %f,y: %f, px: %f, py: %f ---- DISTANCE: %f \n",
       index, m, pts[m].lon, pts[m].lat, p.lon, p.lat, distance);*/

    if (distance < band_width * band_width) {
      f_estimate += GPUKernel(distance / band_width);
    }
  }

  /*printf(
      "index: %d, pts_size: %d, band width: %f, width: %d, height: %d, "
      "estimate: %f, min: %f, max: %f\n",
      index, pts_size, band_width, f_estimate);*/

  f_estimate = f_estimate / (pts_size * band_width * band_width);
  res[index].value = f_estimate;
  res[index].col = index % width;
  res[index].row = index / width;
}

__global__ void CalCulateOpenGLVBO(Item* res, float* data, int width,
                                   int height) {
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  // printf("CalCulateOpenGLVBO: index: %d", index);
  if (index > width * height) return;

  float min = res[0].value;
  float max = res[width * height - 1].value;

  float value = (res[index].value - min) / (max - min);
  int d_index = index * 5;
  int row = res[index].row;
  int col = res[index].col;

  data[d_index + 0] = col;
  data[d_index + 1] = row;
  if (value == 0) {
    data[d_index + 2] = 1;
    data[d_index + 3] = 1;
    data[d_index + 4] = 1;
  } else if (value > 0 && value < 0.4) {
    data[d_index + 2] = 1;
    data[d_index + 3] = 1;
    data[d_index + 4] = 0;
  } else if (value >= 0.4 && value < 0.7) {
    data[d_index + 2] = 0;
    data[d_index + 3] = 1;
    data[d_index + 4] = 0;
  } else if (value >= 0.7 && value < 0.9) {
    data[d_index + 2] = 0;
    data[d_index + 3] = 0;
    data[d_index + 4] = 1;
  } else if (value >= 0.9 && value <= 1) {
    data[d_index + 2] = 1;
    data[d_index + 3] = 0;
    data[d_index + 4] = 0;
  }
}

void PrintCUDAInfo() {
  int dev = 0;
  cudaDeviceProp devProp;
  cudaGetDeviceProperties(&devProp, dev);
  std::cout << "使用GPU device " << dev << ": " << devProp.name << std::endl;
  std::cout << "SM的数量：" << devProp.multiProcessorCount << std::endl;
  std::cout << "每个线程块的共享内存大小："
            << devProp.sharedMemPerBlock / 1024.0 << " KB" << std::endl;
  std::cout << "每个线程块的最大线程数：" << devProp.maxThreadsPerBlock
            << std::endl;
  std::cout << "每个SM的最大线程数：" << devProp.maxThreadsPerMultiProcessor
            << std::endl;
  std::cout << "每个SM的最大线程束数："
            << devProp.maxThreadsPerMultiProcessor / 32 << std::endl;
}

struct sort_functor {
  __host__ __device__ bool operator()(Item left, Item right) const {
    return (left.value < right.value);
  }
};

RendererElement GPUKde(std::vector<Point>& pts, Rect& rect, int width,
                       int height) {
  using namespace std::chrono;
  auto start = high_resolution_clock::now();

  PrintCUDAInfo();

  thrust::host_vector<Point> h_calculation_pts(width * height);
  thrust::device_vector<Point> d_pts(pts);
  thrust::device_vector<Item> d_res(width * height);

  // Calculate band width
  Point avePt = ave(pts);
  float band_width = h(pts, avePt);
  rect.top += band_width;
  rect.bottom -= band_width;
  rect.left -= band_width;
  rect.right += band_width;

  float item_w = (rect.right - rect.left) / width;
  float item_h = (rect.top - rect.bottom) / height;

  // Calculate kernel density estimation
  for (int x = 0; x < width; x++) {
    float item_x = rect.left + item_w * x;

    for (int y = 0; y < height; y++) {
      float item_y = rect.bottom + item_h * y;
      h_calculation_pts[y * width + x] = {item_x, item_y};
    }
  }
  thrust::device_vector<Point> d_calculation_pts(h_calculation_pts);

  dim3 threadsPerBlock(64);
  dim3 blockPerGrid((width * height + threadsPerBlock.x - 1) /
                    threadsPerBlock.x);

  CalCulateKDEKernel<<<blockPerGrid, threadsPerBlock>>>(
      raw_pointer_cast(&d_pts[0]), raw_pointer_cast(&d_calculation_pts[0]),
      raw_pointer_cast(&d_res[0]), band_width, width, height, pts.size());
  if (const cudaError_t err = cudaGetLastError()) {
    std::cout << "CUDA KDE KERNEL ERROR: " << cudaGetErrorString(err) << '\n';
  }
  thrust::stable_sort(d_res.begin(), d_res.end(), sort_functor());

  size_t size = width * height;
  VertexArray* va = new VertexArray();
  VertexBuffer* vb = new VertexBuffer(width * height * 5 * sizeof(float), size);

  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(3);
  // index buffer object, specify how I want to draw this layout.

  float* data;
  cudaGraphicsResourceGetMappedPointer((void**)&data, &size, vb->vbo_cuda);

  CalCulateOpenGLVBO<<<blockPerGrid, threadsPerBlock>>>(
      raw_pointer_cast(&d_res[0]), data, width, height);
  if (const cudaError_t err = cudaGetLastError()) {
    std::cout << "CUDA OPENGL ERROR: " << cudaGetErrorString(err) << '\n';
  }

  cudaGraphicsUnmapResources(1, &vb->vbo_cuda, NULL);
  va->AddBuffer(*vb, layout);
  
  // 4 * 3
  glm::mat4 proj =
      glm::ortho(0.0f, width * 1.0f, 0.0f, height * 1.0f, -1.0f, 1.0f);

  Shader* shader = new Shader("res/shaders/Basic.shader");
  shader->Bind();
  shader->SetUniformMat4f("u_MVP", proj);

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  std::cout << "CALCULATION TIME:: " << duration.count() << " ms" << std::endl;

  return {shader, va, vb, NULL};
}

RendererElement GPUCalculate() {
  // 1. Read file
  auto data = Coordinates("res/data/coord.txt");
  auto pts = data.first;
  auto rect = data.second;
  // 2. Calculate kde
  int width = 15000;
  int height =
      floor(width * (rect.top - rect.bottom) / (rect.right - rect.left));

  RendererElement res = GPUKde(pts, rect, width, height);

  // 3. Return and let renderer to plot
  return res;
}
}  // namespace kde