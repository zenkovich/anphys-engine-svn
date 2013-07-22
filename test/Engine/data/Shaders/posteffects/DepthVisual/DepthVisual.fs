uniform sampler2D texture0;
varying vec2 tc0;

void main(void)
{
  vec4 Depth = vec4(vec3(texture2D( texture0, tc0).r),1.0);

gl_FragColor = Depth;//vec4(Depth,Depth,Depth,1.0);
}