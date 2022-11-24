#shader vertex
#version 330 core

layout(location = 0) in vec3 position;  // specify paramters
layout(location = 1) in vec3 color;

out vec3 out_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0);
  out_color = color;
};

#shader fragment
#version 330 core

out vec4 color;  // specify return value

in vec3 out_color;

void main() { color = vec4(out_color, 1.0); };