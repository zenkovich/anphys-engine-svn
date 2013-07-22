vec3 BlurH(vec2 TexCoord, float StepSize, sampler2D texture)
{
  vec3 Result;
  Result += texture2D(texture, vec2(TexCoord.x - 4.0*StepSize, TexCoord.y)).xyz * 0.05;
  Result += texture2D(texture, vec2(TexCoord.x - 3.0*StepSize, TexCoord.y)).xyz * 0.09;
  Result += texture2D(texture, vec2(TexCoord.x - 2.0*StepSize, TexCoord.y)).xyz * 0.12;
  Result += texture2D(texture, vec2(TexCoord.x - StepSize, TexCoord.y)).xyz * 0.15;
  Result += texture2D(texture, vec2(TexCoord.x, TexCoord.y)).xyz * 0.16;
  Result += texture2D(texture, vec2(TexCoord.x + StepSize, TexCoord.y)).xyz * 0.15;
  Result += texture2D(texture, vec2(TexCoord.x + 2.0*StepSize, TexCoord.y)).xyz * 0.12;
  Result += texture2D(texture, vec2(TexCoord.x + 3.0*StepSize, TexCoord.y)).xyz * 0.09;
  Result += texture2D(texture, vec2(TexCoord.x + 4.0*StepSize, TexCoord.y)).xyz * 0.05;
  return Result;
}