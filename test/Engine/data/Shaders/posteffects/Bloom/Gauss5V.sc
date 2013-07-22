vec3 BlurV(vec2 TexCoord, float StepSize, sampler2D texture)
{
  vec3 Result;
  Result += texture2D(texture, vec2(TexCoord.x, TexCoord.y - 4.0*StepSize)).xyz * 0.05;
  Result += texture2D(texture, vec2(TexCoord.x, TexCoord.y - 3.0*StepSize)).xyz * 0.09;
  Result += texture2D(texture, vec2(TexCoord.x, TexCoord.y - 2.0*StepSize)).xyz * 0.12;
  Result += texture2D(texture, vec2(TexCoord.x, TexCoord.y - StepSize)).xyz * 0.15;
  Result += texture2D(texture, vec2(TexCoord.x, TexCoord.y)).xyz * 0.16;
  Result += texture2D(texture, vec2(TexCoord.x, TexCoord.y + StepSize)).xyz * 0.15;
  Result += texture2D(texture, vec2(TexCoord.x, TexCoord.y + 2.0*StepSize)).xyz * 0.12;
  Result += texture2D(texture, vec2(TexCoord.x, TexCoord.y + 3.0*StepSize)).xyz * 0.09;
  Result += texture2D(texture, vec2(TexCoord.x, TexCoord.y + 4.0*StepSize)).xyz * 0.05;
  return Result;
}              