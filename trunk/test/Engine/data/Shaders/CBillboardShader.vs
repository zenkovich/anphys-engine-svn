varying vec2 TexCoord;
varying vec4 Color;

void main(void)
{
  TexCoord = gl_MultiTexCoord0.xy;
  Color = gl_Color;
  gl_Position = ftransform();
}