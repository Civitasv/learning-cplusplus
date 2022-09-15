## C++ 编译流程

1. 预处理：处理所有 `#` 开头的语句
2. 编译器：将源文件编译为汇编语言
3. 汇编器：将汇编语言编译为目标文件，二进制文件
4. 链接器：将目标文件合在一起生成可执行文件或者库文件
可执行文件/库文件

> 默认是 compile、assemble 和 link。
	1. -E: preprocess only
	2. -S: compile only, don't assemble and link
	3. -c: compile and assemble, don't link

## 规则语法

```make
target ...: prerequisites ...
	recipe
	...
	...
```

规则用于描述目标名称、目标的依赖和如何生成目标。

target: 生成的文件名或执行的命令名，如 clean
prerequisite: 目标 target 所依赖的文件
recipe: 生成目标 target 所需执行的命令

### 默认目标

默认情况下，`make` 执行 makefile 中的第一个目标

要修改默认目标：
1. 使用参数指定其它目标
2. 修改 `.DEFAULT_GOAL` 变量值

### 伪目标

```make
clean:
	rm *.o
```

伪目标仅用于执行一些命令，如 `clean` 用于删除所有的 `.o` 目标文件。

可以使用 `.PHONY` 显式指示该目标为伪目标。

## 变量

```make
objects = main.o kbd.o command.o display.o \
          insert.o search.o files.o utils.o

edit : $(objects)
        cc -o edit $(objects)
```


## link libraries

```sh
gcc -o main main.c -L/usr/local/lib/ -lYARP_OS
```

上述命令中：

- `-L` 用于指定 libraries 文件夹
- `-l` 用于指定要 link 的 library 的名称

## include headers

```sh
gcc -Iinclude/
```

## 脚本

0. 开场
	大家好啊，今天我将对 makefile 进行简单的介绍。
	首先，为什么需要 makefile，以下面这个 C++ 文件为例：
1. C++编译流程(1 min)
	
	我们可以使用 `g++ main.cpp -o output` 可以编译源文件为可执行文件，执行`./output` 可以得到结果。但这样有几个问题：
	1. 每次编译，需要重复编写上述命令；
	2. 随着项目的体量的增加，文件数量会变多，依赖关系会更复杂；
	3. 编译速度减慢。
2. 什么是 MakeFile
	简单的说，makefile 的作用为：
	1. 定义目标
	2. 定义目标依赖的文件
	3. 定义生成目标所需执行的命令。

	makefile 的语法十分简单：

	```make
	target ...: prerequisites ...
		recipe
		...
		...
	```
	
	总体被称为一条规则，其用于描述目标名称、目标所依赖的文件和生成目标所需执行的命令。

	使用 `make` 命令即可编译所需目标，这样，我们就无需重复编写命令，同时，makefile 也能帮助我们管理依赖关系，在构建时，`make` 还会根据依赖关系智能判断 makefile 中的哪些目标需要重新编译，从而显著减少编译所需的时间。
	
	**强调一遍，makefile 并不负责编译，所执行的命令负责编译，比如对于 C/C++，我们还必须使用 gcc 或 clang 编译器，而 makefile 则用于管理这些命令。**

看一个例子。

```makefile
run: main.o another.o
	g++ main.o another.o -o output

main.o: main.cpp
	g++ -c main.cpp

another.o: another.cpp
	g++ -c another.cpp

clean:
	rm *.o output
```

注意到，我们并没有使用 `g++ main.cpp another.cpp -o output`，这样做的目的是加快编译速度。

C++ 的编译流程为：
1. 预处理：处理所有宏命令
2. 编译器：将源文件编译为汇编语言
3. 汇编器：将汇编语言编译为目标文件，二进制文件
4. 链接器：将目标文件合在一起生成可执行文件或者库文件

接着我们看一个有外部依赖的例子：

```makefile
├─Dependencies
│  │
│  └─GLFW
│      │
│      ├─include
│      │  └─GLFW
│      │      glfw3.h
│      │      glfw3native.h
│      │
│      └─lib
│          libglfw3.a
│
└─OpenGL
    │
    └─src
		Application.cpp
```

此时根目录 MakeFile 应为：

```makefile
app: Application.o
	g++ Application.o -LDependencies/GLFW/lib -lglfw3 -lX11 -lOpenGL -o app

Application.o: OpenGL/src/Application.cpp
	g++ -IDependencies/GLFW/include -c OpenGL/src/Application.cpp

clean:
	rm *.o app
```

其中 `-L` 用于指定链接目录，`-l` 指定链接库，`-I` 指定头文件目录。

结束语：
以上是本视频的全部内容，希望能让你对 makefile 有所了解，如果有任何疑问，欢迎在评论区内提出。
