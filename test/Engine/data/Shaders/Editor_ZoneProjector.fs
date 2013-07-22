uniform sampler2D depthMap;
uniform sampler2D zonesMap;

uniform vec2 LeftTop;
uniform vec2 RightBottom;
uniform vec2 Size;

varying vec2 tc0;
varying vec2 ScreenPos;

vec4 Unproject(){
  vec4 _in = vec4(ScreenPos.xy,2.0 * texture2D ( depthMap, tc0 ).x - 1.0,1.0);
  vec4 _out = gl_ModelViewProjectionMatrixInverse*_in;
  _out.x = _out.x/_out.w;
  _out.y = _out.y/_out.w;
  _out.z = _out.z/_out.w; 
  return _out;
}

void main(void)
{
  vec2 Pos = Unproject().xy;
  Pos.x = (Pos.x-LeftTop.x)/RightBottom.x;
  Pos.y = (Pos.y-LeftTop.y)/RightBottom.y;
  
  float fx = abs(Pos.x - Size.x*floor(Pos.x/Size.x));
  float fy = abs(Pos.y - Size.y*floor(Pos.y/Size.y));
  
  vec3 Color = texture2D(zonesMap,Pos).xyz;
  if (fx<Size.x/10.0 || fy<Size.y/10.0)
    Color = vec3(1.0);
  if (Pos.x<0.0 || Pos.x>1.0 || Pos.y<0.0 || Pos.y>1.0)	
    Color = vec3(0.5,0.0,0.0);
  gl_FragColor = vec4(Color,0.4);
}