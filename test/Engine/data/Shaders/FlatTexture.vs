varying vec4 Color;
varying vec2 TexCoord;

void main(void)
{
  Color = gl_Color;
  TexCoord = gl_MultiTexCoord0.xy;
  gl_Position = ftransform();
}