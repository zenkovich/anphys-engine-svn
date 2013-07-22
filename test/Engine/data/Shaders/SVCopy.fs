uniform sampler2D texture;
varying vec2 tc0;

void main(void)
{
  vec4 Color = texture2D( texture, tc0, 1.0 );
  gl_FragColor = vec4(Color.xyz,1.0);
}