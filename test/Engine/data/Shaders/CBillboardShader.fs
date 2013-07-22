uniform sampler2D Texture;

varying vec2 TexCoord;
varying vec4 Color;

void main(void)
{
  gl_FragColor = Color*texture2D(Texture,TexCoord);
}