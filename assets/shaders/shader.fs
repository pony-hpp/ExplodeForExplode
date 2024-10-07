#version 330 core

in vec2 vTexPos;

out vec4 fColor;

uniform sampler2D uTex;

void main() {
  fColor = texture(uTex, vTexPos);
}
