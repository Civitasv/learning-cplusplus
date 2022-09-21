#shader vertex
#version 330 core

layout(location = 0) in vec4 position; // specify paramters

void main()
{
  gl_Position = position;
};

#shader fragment
#version 330 core

out vec4 color; // specify return value

void main()
{
  color = vec4(0.2, 0.3, 0.0, 1.0);
};