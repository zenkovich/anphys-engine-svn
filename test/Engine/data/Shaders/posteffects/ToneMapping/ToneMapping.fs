uniform sampler2D texture0;    //MAIN
varying vec2 tc0;
varying float factor;

void main(void)
{
  vec3 Color = texture2D(texture0,tc0).rgb*factor;
  gl_FragColor = vec4(Color,1.0);
}