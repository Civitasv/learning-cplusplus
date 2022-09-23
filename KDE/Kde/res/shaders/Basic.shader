#shader vertex
#version 330 core

layout(location = 0) in vec2 position;  // specify paramters
layout(location = 1) in vec3 color;

out vec3 v_Color;

uniform mat4 u_MVP;

void main() {
  gl_Position = u_MVP * vec4(position, 1.0, 1.0);
  v_Color = color;
};

#shader fragment
#version 330 core

out vec4 color;  // specify return value
in vec3 v_Color;

void main() { color = vec4(v_Color, 1.0); };