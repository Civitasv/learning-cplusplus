CPU运算核心少，但是可以实现复杂的逻辑运算，适合控制密集型任务，此外，CPU 上的线程是重量级的，上下文切换开销很大。

GPU运算核心很多，适合计算密集型任务，此外，线程是轻量级的。

因此，基于 CPU ＋ GPU 的异构计算平台可以优势互补，使用 CPU 处理逻辑复杂的串行程序，使用 GPU 处理计算密集型的并行计算程序，从而发挥最大功效。

host: CPU 及其内存
device: GPU 及其内存

典型的 CUDA 程序执行流程：
1. 分配 host 内存，并进行数据初始化；
2. 分配 device 内存，并从 host 将数据拷贝到 device 上；
3. 调用 CUDA 的核函数在 device 上完成指定的运算；
4. 将 device 的运算结果拷贝到 host 上（只是可视化的话，就可以不拷贝？直接用这个显示）；
5. 释放 device 和 host 上分配的内存。

kernel: `<<<grid, block>>>`

`__global__`: 在 device 上执行，从 host 调用；
`__device__`: 在 device 上执行，仅可从 device 上调用；
`__host__`: 在 host 上执行，可省略不写。

一个 kernel 启动的所有线程称为一个网格（grid），一个网格包含很多 Block，每个 Block 包含很多 Thread，同一个网格上的线程共享相同的全局内存空间。

```c
dim3 grid(3, 2);
dim3 block(5, 3);
kernel_fun<<< grid, block >>>(prams...);
```


![[Pasted image 20220924151518.png]]

所以，一个线程需要两个内置的坐标变量（blockIdx，threadIdx）来唯一标识，它们都是`dim3`类型变量，其中blockIdx指明线程所在grid中的位置，而threaIdx指明线程所在block中的位置，如图中的Thread (1,1)满足：

```text
threadIdx.x = 1
threadIdx.y = 1
blockIdx.x = 1
blockIdx.y = 1
```

PDF: probability density functions
KDE: kernel density estimation, a non-parametric statistical procedure to estimate PDFs from samples.

一个带宽 vs 多个带宽

CUDA vs OpenCL

host-device design, host, eg, the main CPU, communicates with a device, eg, a GPU.

![[Pasted image 20220926105234.png]]

Kernel: 一段由 device 端执行的代码

![[Pasted image 20220926105831.png]]

![[Pasted image 20220926112419.png]]

看看 Thrust 或者 ArrayFire，先使用 CUDA