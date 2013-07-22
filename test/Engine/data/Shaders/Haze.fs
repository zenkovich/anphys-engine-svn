uniform sampler2D texture;
uniform sampler2D noise;
uniform sampler2D reflected;
uniform sampler2D depth;
varying vec2 tc0;

const float Reflections = 0.2;

void main(void)
{
float distortPower = 0.02;
vec4 noize = texture2D( noise, tc0 );
vec4 n = 2.0 * noize - 1.0;
vec2 newTC = n.xy * distortPower + tc0;

float IsWater = 0.0;
if (noize.z>0.0)
  IsWater = 1.0;
float currentdepth = texture2D( depth, tc0, 1.0 ).r;  
float newdepth = texture2D( depth, newTC, 1.0 ).r;  
vec4 color = vec4(1.0);
if (currentdepth<=newdepth || IsWater==0.0)
  color = texture2D( texture, tc0, 1.0 ) + texture2D( reflected, tc0, 1.0 )*Reflections*IsWater;
else  
  color = texture2D( texture, newTC, 1.0 ) + texture2D( reflected, newTC, 1.0 )*Reflections;
gl_FragColor = color;
}