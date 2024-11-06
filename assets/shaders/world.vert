#version 330 core

layout(location = 0) in vec2 iPos;
layout(location = 1) in vec2 iTexPos;

out vec2 vPos;
out vec2 vTexPos;
flat out uint vViewportH;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
  gl_Position = uProjection * uView * vec4(iPos, 0.0f, 1.0f);
  vPos        = iPos;
  vTexPos     = iTexPos;
  vViewportH  = uint(uProjection[1][1] * 2);
}
