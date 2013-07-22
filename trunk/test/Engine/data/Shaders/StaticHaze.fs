varying vec2 texCoord;
varying vec4 glColor;

uniform sampler2D tex_Diffuse;
//include "GFog_fs.sc"

void main (void)
{
  vec4 ResultColor;// = vec4(1.0,1.0,1.0,1.0);
  vec4 base = texture2D(tex_Diffuse, texCoord)*glColor;	
  ResultColor = MixWithFog(base);
  gl_FragColor = ResultColor;
}