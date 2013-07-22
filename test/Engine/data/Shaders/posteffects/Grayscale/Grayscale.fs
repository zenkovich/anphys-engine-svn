uniform sampler2D texture0;
varying vec2 tc0;

void main(void)
{
  vec4 Color = texture2D( texture0, tc0, 1.0 );
  float Gray = max(Color.r,max(Color.g,Color.b));
  gl_FragColor = vec4(Gray,Gray,Gray,1.0);
}