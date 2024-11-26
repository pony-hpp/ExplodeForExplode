#version 330 core

in vec2 vPos;
in vec2 vTexPos;
flat in uint vViewportH;

out vec4 fColor;

uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform uint uWorldH;
uniform float uDurability;

void main()
{
  vec4 blockPx      = texture(uTex0, vTexPos);
  vec4 durabilityPx = texture(uTex1, vTexPos);

  float blackFactor = (vViewportH - vPos.y) / uWorldH + 1.0f - 0.2f;
  blockPx = mix(blockPx, vec4(0.0f, 0.0f, 0.0f, 1.0f), max(0.0f, blackFactor));

  float durabilityAlpha = durabilityPx.a * (1.0f - uDurability);
  fColor.rgb            = blockPx.rgb - durabilityAlpha;

  fColor.a = blockPx.a;
}
