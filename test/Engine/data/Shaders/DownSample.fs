uniform sampler2D texture;
varying vec2 tc0;

const float Luminance = 0.08;
const float fMiddleGray = 0.18;
const float fWhiteCutoff = 0.8;

vec3 BrightPassFilter( vec3 tex )
{
    vec3 ColorOut = tex; 

    ColorOut = ColorOut * fMiddleGray / ( Luminance + 0.001 );
    ColorOut = ColorOut * ( 1.0 + ( ColorOut / ( fWhiteCutoff * fWhiteCutoff ) ) );
    ColorOut = ColorOut - 5.0;

    ColorOut = max( ColorOut, 0.0 );

    ColorOut /= ColorOut;

    return ColorOut;
}

void main(void)
{
  vec3 Color = texture2D( texture, tc0, 1.0 ).rgb;
  //gl_FragColor = vec4(BrightPassFilter(Color),1.0);
  gl_FragColor = vec4(Color,1.0);
}