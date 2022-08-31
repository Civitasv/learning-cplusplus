解决方案文件(sln)：一个或多个项目的集合

添加 include_dir 时，只会识别 .h 头文件，因此编译到 .obj 并没有问题，但是实现文件 .cpp 无法被识别，因此需要通过 `add existing item` 添加对应的源文件，使 VS 能够识别。

添加 library_dir 时，需要在 input 中指定需要链接的库，VS 不支持单文件链接。

![[Pasted image 20220831192023.png]]

![[Pasted image 20220831192048.png]]
