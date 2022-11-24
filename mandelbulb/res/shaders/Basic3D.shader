#shader vertex
#version 330 core

layout(location = 0) in vec2 position;  // specify paramters
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main() {
  gl_Position = u_MVP * vec4(position, 1.0, 1.0);
  v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

out vec4 color;  // specify return value
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() { color = texture(u_Texture, v_TexCoord); };