varying vec2 texCoord;

uniform sampler2D tex_Diffuse;
	
void main (void)
{
    vec4 base = texture2D(tex_Diffuse, texCoord);
	base.w = 1.0;	
  gl_FragColor = base;
}