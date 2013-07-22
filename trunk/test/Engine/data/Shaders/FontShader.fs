varying vec2 TexCoord;
varying vec4 Color;
uniform sampler2D FontTexture;
void main(void)
{  
  gl_FragColor = vec4(Color.r,Color.g,Color.b,Color.a*texture2D(FontTexture,TexCoord).r);
}