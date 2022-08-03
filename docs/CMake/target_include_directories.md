## command

```cmake
target_include_directories(<target> [SYSTEM] [AFTER|BEFORE]
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

该命令用于为特定目标 `target` 添加头文件目录。

**参数说明：**

1. `<target>`: 构建目标，使用 `add_executable()` 或 `add_library` 声明，不可为 `ALIAS` 目标；
2. `[SYSTEM]`: 指定是否平台相关，若该属性与 `PUBLIC` 或 `INTERFACE` 一同设置，CMake 将使用指定的目录初始化目标的 `INTERFACE_SYSTEM_INCLUDE_DIRECTORIES` 属性；
	1. `INTERFACE_SYSTEM_INCLUDE_DIRECTORIES`: 该属性为含有平台相关头文件的目录，常用于抑制编译器警告；
3. `[AFTER|BEFORE]`: 指定相应目录添加的顺序，在后添加/在前添加。
4. `[INTERFACE|PUBLIC|PRIVATE]`: 指定头文件的可见性，若设置 `PRIVATE` 或 `PUBLIC`，CMake 将使用指定的目录初始化目标的 `INCLUDE_DIRECTORIES` 属性，若设置为 `PUBLIC` 或 `INTERFACE`，CMake 将使用指定的目录初始化目标的 `INTERFACE_INCLUDE_DIRECTORIES` 属性；
	1. `INCLUDE_DIRECTORIES`: 该属性为目标的头文件目录；
	2. `INTERFACE_INCLUDE_DIRECTORIES`: 该属性为目标的开放头文件目录；
5. `[items]`: 指定要添加的头文件目录，可以使用相对路径或绝对路径，相对路径为相对于当前路径 `CMAKE_CURRENT_SOURCE_DIR`，也可以使用 CMake 提供的 generator expression。

头文件的使用在构建和安装时往往是不同的，CMake 提供 `BUILD_INTERFACE` 和 `INSTALL_INTERFACE` 用于分别指定构建时和安装时的路径。
1. `BUILD_INTERFACE`: 一方面，可以用来指定构建时引用头文件的 base 路径，应使用绝对路径，如 `$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>` 指定项目根目录为 base 路径，另一方面，CMake 执行 `export` 时，将使用到该属性；
2. `INSTALL_INTERFACE`: 指定安装路径，应使用相对路径，CMake 使用安装路径与该路径拼接得到实际安装路径。

## 举例

假设有以下项目结构：

```cmake
├─lib
│  │  CMakeLists.txt
│  │
│  └─pub
│      │  CMakeLists.txt
│      │
│      └─linux
│              CMakeLists.txt
│              lib.h
│
├─runtime
│      CMakeLists.txt
│      Main.cpp
│
└─test
    │  CMakeLists.txt
    │  Test.cpp
    │
    └─pub
            CMakeLists.txt
            Test.h
```

其中依赖关系为：
1. Main.cpp 引用 Test.h；
2. Test.h 引用 lib.h；
3. lib.h 是一个头文件库。

### Case 1

希望在 `Main.cpp` 中使用 `#include "test/pub/Test.h"` ，在 `Test.h` 中使用 `#include "lib/pub/linux/lib.h"` 引用头文件，则需要对目标 `runtime` 和 `test`：

```cmake
target_include_directories(runtime PUBLIC
    ${PROJECT_SOURCE_DIR}
    )

# 或者

target_include_directories(test PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    )
```

注意，经测试，只要不使用 `export` 命令，使用 `$<BUILD_INTERFACE>` 或不使用都是没有关系的，但据我观察，项目里几乎都会加上 `$<BUILD_INTERFACE>`。

### Case 2

希望在 `Main.cpp` 中直接使用 `#include "Test.h"`，在 `Test.h` 中使用 `#include "linux/lib.h"`，则：

```cmake
target_include_directories(runtime PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/test/pub
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/lib/pub
    )

# 或者

target_include_directories(test PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/lib/pub
    )
```

注意，这里对 `runtime` 需要添加两条目录，因为 C++ 中对于 `#include` 命令是直接把其内容复制过来的，若不添加，编译器在解析文件 `Main.cpp` 时，因为目标 `runtime` 仅包含了 `test/pub` 目录，所以是没办法解析 `linux/lib.h` 的，因此还要添加一行。

所以，个人感觉在实际项目中选用 Case 1 中的做法会好一些。