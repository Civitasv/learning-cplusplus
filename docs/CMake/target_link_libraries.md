CMake 在 link 到 library 时，如果直接使用源码。假设有两个 library A 和 B。

A 头文件在 a_include，B 头文件在 b_include，分别使用 [[target_include_directories]] 命令进行了头文件引入，那么如果 A 使用 `target_link_libraries` 链接到 B，那么 A 也有 B 头文件的访问权。
