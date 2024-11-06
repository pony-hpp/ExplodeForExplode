#version 330 core

in vec2 vPos;
in vec2 vTexPos;
flat in uint vViewportH;

out vec4 fColor;

uniform sampler2D uTex;
uniform uint uWorldH;

void main()
{
  vec4 pix = texture(uTex, vTexPos);

  fColor = mix(
    pix, vec4(0.0f, 0.0f, 0.0f, 1.0f),
    vPos.y <= uWorldH ? ((vViewportH - vPos.y) / uWorldH) + 1.0f : 0.0f
  );
  fColor.a = pix.a;
}
