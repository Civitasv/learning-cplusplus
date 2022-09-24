OpenGL 只是一个规范，不同的 GPU 厂商有自己的实现。

OpenGL 是一个状态机，前面在 buffer 里绑定数据，状态就改变了，后面就可以使用，绘制的也是根据当前所绑定的 buffer 决定的。

OpenGL Shader:
十分类似于 C++
1. compile
2. link

index buffer: 用于更有效地利用内存

vertex array object 里面包括 index buffer object，vertex buffer object 以及 vertex layout 里面的数据，可以通过绑定 vertex array object 绑定 index buffer object 和 vertex buffer object 和 vertex layout。

![[Pasted image 20220924100532.png]]
本来 LEGACY_OPENGL 和 MODERN_OPENGL 时间不一样，修改了这个就一样了...