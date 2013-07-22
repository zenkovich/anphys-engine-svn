uniform sampler2D texture0; //MAIN
uniform sampler2D texture1; //SELF

uniform float dt; //DELTA TIME
uniform float ChangeTime;

uniform float BrightFactor;

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

    ColorOut /= ( vec3(BrightFactor) + ColorOut );

    return ColorOut;
}

void main(void)
{
  
  vec3 OldColor = texture2D( texture1, tc0, 1.0 ).xyz;
  vec3 CurrentColor = BrightPassFilter(texture2D( texture0, tc0, 1.0 ).xyz);
  vec3 Color = mix(OldColor,CurrentColor, dt/ChangeTime);
  //Color = vec3(log(dot(texture2D( texture0, tc0, 1.0 ).xyz,LUMINANCE_VECTOR) + 0.0001));
  //Color = texture2D( texture0, tc0, 1.0 ).xyz;
  
  gl_FragColor = vec4(Color,1.0);
}