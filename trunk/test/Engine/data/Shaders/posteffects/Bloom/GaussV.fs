uniform sampler2D texture0;
uniform float ySize;
varying vec2 tc0;

//include "Gauss5V.sc"

void main(void)
{
  gl_FragColor = vec4(BlurV(tc0, ySize, texture0),1.0);
}