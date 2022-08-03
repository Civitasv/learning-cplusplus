CMake 中，`<>` 表示必选参数，`[]` 表示可选参数，例如：

```cmake
target_include_directories(<target> [SYSTEM] [AFTER|BEFORE]
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

其中 `target`, `INTERFACE|PUBLIC|PRIVATE` 为必选参数。

## Content

1. [[cmake-commands]]
2. [[cmake-modules]]
3. [[cmake-generator-expressions]]