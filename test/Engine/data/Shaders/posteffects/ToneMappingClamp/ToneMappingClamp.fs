uniform sampler2D texture0;    //MAIN
varying vec2 tc0;

void main(void)
{
  vec4 Color = texture2D(texture0,tc0);
  gl_FragColor =   Color / (  Color + vec4(1.0)) ;//vec4(clamp(Color,vec3(0.0),vec3(1.0)),1.0);
}