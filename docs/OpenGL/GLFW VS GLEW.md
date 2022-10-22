GLFW, GLUT, SDL, SFML etc.. are libraries used to create and manage OpenGL contexts and windows. You need this before you can use OpenGL. Some of these libraries provide support for cross platform input and sound stuff as well.

GLEW, GLAD, glLoadgen etc.. are OpenGL extension loaders. Since OpenGL support is implementation specific and constantly updating, these loaders query which OpenGL specs and extensions are supported by the drivers installed, or which you want to use, and provide you a way to access that OpenGL functionality.

> from [GLEW, GLU, GLFW. Which should I use? : gamedev (reddit.com)](https://www.reddit.com/r/gamedev/comments/45v9uz/glew_glu_glfw_which_should_i_use/)


A detailed answer: https://stackoverflow.com/a/61685993/16991209

GLEW (or OpenGL Extension Wrangler) is pretty much a loader, that helps you to manage OpenGl in an "easy" way. In general, functions of the OpenGL library are already on your computer, contained in binary form (.dll) inside your graphics drivers, and GLEW is a *portal* between you and OpenGl that *already lives there*. It provides the functionality of OpenGL in such a way, that is convenient for you to use. That's what GLEW developers mean by "load a pointer to function" - to access a graphics driver of your computer and load specific functions from it so that you can use them in your C++ code.

**Note**: You can actually do what GLEW does by yourself, as t.niese pointed out in the comment section, but that's quite a challenge for a beginner, and therefore it's more convenient to use GLEW.

GLFW (or Graphics Library Framework) on the other hand is a library that helps you to manage such things as window, its size, params, basic input and events-stuff. GLEW needs a window and a valid OpenGL context to be setup, and that's exactly where GLFW comes in handy. 

What is also interesting that these libraries were made not just to make your developing life easier, but also your deployment faster. GLEW and GLFW are kind of cross-platform so you don't have to write different versions of the same application on different platforms.

As practice shows, GLEW/GLAD + GLFW is a common use for modern computer graphics based on OpenGL. You *could*, however, choose other libraries for a reason, based on what you need. But the general approach is to use these, because they support the latest versions of OpenGL, and always get updated.

If you are new to OpenGL and want to learn its basics, you should check this [youtube playlist](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2). There is a simple and quite fast OpenGL tutorial, and there are things that you should definitely know as a developer. Have a nice time!


----------
**EDIT**

GLUT and GLFW are basically the same, but what you need to know is that the original GLUT has been unsupported for 20 years, and there is a free open-source alternative for it called freeglut. So, I guess that is what you mean by "deprecated" :)