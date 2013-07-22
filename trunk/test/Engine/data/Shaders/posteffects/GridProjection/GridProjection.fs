uniform sampler2D texture0;  //FBO
uniform sampler2D texture1;  //DEPTH

uniform float GridSize;
uniform float LineWidth;
uniform vec4 LineColor;

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
  Pos.x = fract(abs(Pos.x)/GridSize);
  Pos.y = fract(abs(Pos.y)/GridSize);
  
  if (Pos.x<LineWidth || Pos.x>GridSize-LineWidth ||
      Pos.y<LineWidth || Pos.y>GridSize-LineWidth)
	  Color = mix(Color,vec4(LineColor.xyz,1.0),LineColor.a);
  gl_FragColor = Color;
}