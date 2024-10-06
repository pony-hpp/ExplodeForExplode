#version 330 core

layout(location = 0) in vec2 iPos;

uniform mat4 uProjection;

void main() {
  gl_Position = uProjection * vec4(iPos, 0.0f, 1.0f);
}
