#version 330 core

in vec2 vPos;
in vec2 vTexPos;

out vec4 fColor;

uniform sampler2D uTex;
uniform uint uPondTop;
uniform uint uPondBottom;

void main()
{
  vec4 pix = texture(uTex, vTexPos);

  float blackFactor = (uPondTop - vPos.y) / (uPondTop - uPondBottom) + 0.5f;
  fColor = mix(pix, vec4(0.01f, 0.01f, 0.1f, 1.0f), max(0.0f, blackFactor));

  if (pix.a == 0.0f)
  {
    fColor.a = 0.0f;
  }
}
