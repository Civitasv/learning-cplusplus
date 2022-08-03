该表达式会在构建期间调用。

```cmake
target_include_directories(tgt PRIVATE /opt/include/$<CXX_COMPILER_ID>)
```

`$<CXX_COMPILER_ID>` 会根据使用编译器的不同产生不同的值。
