uniform sampler2D texture0;
uniform float xSize;
varying vec2 tc0;

//include "Gauss5H.sc"

void main(void)
{
  gl_FragColor = vec4(BlurH(tc0, xSize, texture0),1.0);
}