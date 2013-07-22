#version 150

uniform sampler2D Diffuse;

in vec2 texCoord;

out vec4 color;

void main(void)
{
  vec4 Result = texture2D(Diffuse, texCoord);
  color = Result;
}