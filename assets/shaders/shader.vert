#version 330 core

layout(location = 0) in vec2 iPos;
layout(location = 1) in vec2 iTexPos;

out vec2 vTexPos;

uniform mat4 uView;
uniform mat4 uProjection;

void main() {
  gl_Position = uProjection * uView * vec4(iPos, 0.0f, 1.0f);
  vTexPos     = iTexPos;
}
