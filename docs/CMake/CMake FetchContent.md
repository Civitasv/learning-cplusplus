---
author: 胡森
date: 2022/05/25
email: hscivitasv@gmail.com
---

该模块用于获取外部依赖库，在构建生成文件过程中被调用，接着，CMakeLists.txt 文件中的其它命令（如 **add_subdirectory**, **include**, **file**）可以立即使用其所获取的依赖。

该模块主要包括四个命令。
1. *FetchContent_Declare(\<name\> \<contentOptions\>...)*: 描述如何下载依赖库，**name** 声明下载库的名称，**contentOptions** 描述获取、更新外部库的方式（常用的有通过 Git Repo下载，通过 URL 下载等）；
2. *FetchContent_MakeAvaliable(\<name1> \[\<name2>...])*: 构建命令1所声明的 **name1, name2, ...**；
3. *FetchContent_Populate(\<name>)*: 构建命令1所声明的 **name** 依赖， 该命令会定义三个变量：
	1. *\<lowercaseName>\_POPULATED*: 依赖是否已被构建
	2. *\<lowercaseName>\_SOURCE_DIR*: 依赖存储路径
	3. *\<lowercaseName>\_BINARY_DIR*: 依赖 Build 路径
	
	若 *FetchContent_Populate* 不止有 name 参数，此时，不再使用 *FetchContent_Declare* 所定义的配置，而是由 *FetchContent_Populate* 给出，不再定义 *\<lowercaseName>\_POPULATED*，不会在全局定义*\<lowercaseName>\_SOURCE_DIR* 和 *\<lowercaseName>\_BINARY_DIR*，但仍会在当前作用域内定义，不再检测是否已经构建该依赖，具体语法为:
	
	```cmake
	FetchContent_Populate(
	  <name>
	  [QUIET]
	  [SUBBUILD_DIR <subBuildDir>]
	  [SOURCE_DIR <srcDir>]
	  [BINARY_DIR <binDir>]
	  ...
	)
	```
	
	其中，**QUIET** 表示隐藏与激活依赖相关的输出；**SUBBUILD_DIR** 用于指定 sub-build 路径；**SOURCE_DIR** 用于指定 source 路径，**BINARY_DIR** 用于指定 binary 路径，其余参数均会传递给 [ExternalProject_Add()](https://cmake.org/cmake/help/latest/module/ExternalProject.html#command:externalproject_add "externalproject_add")命令。
4. *FetchContent_GetProperties(\<name> \[SOURCE_DIR \<srcDirVar>] \[BINARY_DIR \<binDirVar>] \[POPULATED \<doneVar>])*: 获取与外部依赖 **name** 相关的属性。

## 应用场景

以 [googletest](https://github.com/google/googletest) 为例，使用 *FetchContent* 模块配置如下。
1. 声明下载依赖库，指定获取方式为 git:

	```cmake
	include(FetchContent)
	FetchContent_Declare(
	  googletest
	  GIT_REPOSITORY https://github.com/google/googletest.git
	  GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
	)
	```
	
2. 下载 googletest，使其可用：
	
	```cmake
	FetchContent_MakeAvailable(googletest myCompanyIcons)
	```

上述方式将库的声明与实际的下载分离，利于项目管理和维护。

一般而言，*FetchContent_Declare* 和 *FetchContent_MakeAvailable* 已足够应付大多数情况，若需要更灵活的控制库的声明和下载，可以结合使用 *FetchContent_Populate* 和 *FetchContent_GetProperties*，如下例所示。

```cmake
# NOTE: Where possible, prefer to use FetchContent_MakeAvailable()
#       instead of custom logic like this

# Check if population has already been performed
FetchContent_GetProperties(depname)
if(NOT depname_POPULATED)
  # Fetch the content using previously declared details
  FetchContent_Populate(depname)

  # Set custom variables, policies, etc.
  # ...

  # Bring the populated content into the build
  add_subdirectory(${depname_SOURCE_DIR} ${depname_BINARY_DIR})
endif()
```

## 最佳实践

1. 总是优先使用 *FetchContent_MakeAvailable* 而不是 *FetchContent_Populate*。
2. 总是优先使用 *FetchContent_Declare* 声明所有依赖，再使用 *FetchContent_MakeAvailable* 或 *FetchContent_Populate* 构建依赖，确保当前项目能够控制依赖的具体内容，这是因为 *FetchContent_Declare* 不会覆盖先前声明，因此，当子项目与父项目有相同依赖时，子项目不会覆盖父项目的声明。假设依赖 **use_other** 也使用 *FetchContent* 声明依赖 **other**，我们需要：
	
	```cmake
	# CORRECT: All details declared first, so they will take priority
	FetchContent_Declare(uses_other ...)
	FetchContent_Declare(other ...)
	FetchContent_MakeAvailable(uses_other other)
	```
	
	而不是：

	```cmake
	# WRONG: Should declare all details first
	FetchContent_Declare(uses_other ...)
	FetchContent_MakeAvailable(uses_other)
	
	FetchContent_Declare(other ...)    # Will be ignored, uses_other beat us to it
	FetchContent_MakeAvailable(other)  # Would use details declared by uses_other
	```
	
3. *FetchContent_MakeAvailable* 会先检查依赖是否已经构建完成，因此不会重复构建，但 *FetchContent_Populate* 并不会，重复构建会报错，因此， 使用 *FetchContent_Populate* 前，必须按照上述示例，使用 *FetchContent_GetProperties* 获取变量 *\<lowercaseName>\_POPULATED*，检测是否需要构建该依赖。

## 参考

1. [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html#id1)
2. [ExternalProject](https://cmake.org/cmake/help/latest/module/ExternalProject.html#module:ExternalProject "ExternalProject")