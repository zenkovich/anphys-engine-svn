uniform sampler2D texture0;  //FBO
uniform sampler2D texture1;  //DEPTH
uniform sampler2D texture2;  //SHADOW

uniform vec2 Direction;
uniform float Speed;
uniform vec4 ShadowColor;
uniform float ShadowSize;

uniform float Time;

varying vec2 tc0;
varying vec2 ScreenPos;

vec4 Unproject(){
  vec4 _in = vec4(ScreenPos.xy,2.0 * texture2D ( texture1, tc0 ).x - 1.0,1.0);
  vec4 _out = gl_ModelViewProjectionMatrixInverse*_in;
  _out.x = _out.x/_out.w;
  _out.y = _out.y/_out.w;
  _out.z = _out.z/_out.w; 
  return _out;
}

void main(void)
{
  vec4 Color = texture2D( texture0, tc0, 1.0 );
  vec2 Pos = Unproject().xy;
  Pos = Pos + vec2(Time*Speed)*Direction;
  //Pos.x = fract(abs(Pos.x)/GridSize);
  //Pos.y = fract(abs(Pos.y)/GridSize);
  
  float Shadow = texture2D( texture2, Pos/vec2(ShadowSize) ).r;
  
  Color = mix(Color,vec4(ShadowColor.xyz,1.0),Shadow*ShadowColor.w);
  gl_FragColor = Color;
}