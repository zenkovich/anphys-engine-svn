uniform sampler2D texture;
varying vec4 Color;
varying vec2 TexCoord;

void main(void)
{
  vec4 Result = texture2D(texture, TexCoord);
  gl_FragColor = Result*Color;
}