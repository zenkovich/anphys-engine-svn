uniform sampler2D texture0;    //MAIN
varying vec2 tc0;

void main(void)
{
  vec4 Color = texture2D(texture0,tc0);
  gl_FragColor = Color;
}