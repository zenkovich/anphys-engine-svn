uniform sampler2D texture0;    //MAIN
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
varying vec2 tc0;

uniform float Brightness;

//include "Gauss5V.sc"

void main(void)
{
  vec3 Bloom = texture2D(texture1,tc0).rgb+
               texture2D(texture2,tc0).rgb+
               texture2D(texture3,tc0).rgb+
               texture2D(texture4,tc0).rgb;
			   
  vec3 Color = texture2D(texture0,tc0).rgb+Bloom*Brightness;
  gl_FragColor = vec4(Color,1.0);
}