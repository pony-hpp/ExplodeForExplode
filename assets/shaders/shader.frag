#version 330 core

in vec2 vPos;
in vec2 vTexPos;
in float vViewportH;

out vec4 fColor;

uniform sampler2D uTex;
uniform uint uWorldH;

void main()
{
  fColor = mix(
    texture(uTex, vTexPos), vec4(0.0f, 0.0f, 0.0f, 1.0f),
    ((vViewportH - vPos.y) / uWorldH) + 1.0f
  );
}
