uniform sampler2D texture0;  //FBO
uniform sampler2D texture1;  //Haze
varying vec2 tc0;

uniform float distortPower;

void main(void)
{
//float distortPower = 0.0532;
vec4 noise = texture2D( texture1, tc0 );
vec4 n = 2.0 * noise - 1.0;
vec2 newTC = n.xy * distortPower + tc0;

gl_FragColor = texture2D( texture0, newTC, 1.0 );
}