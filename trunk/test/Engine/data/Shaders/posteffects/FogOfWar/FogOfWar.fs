uniform sampler2D texture0;  //FBO
uniform sampler2D texture1;  //DEPTH
uniform sampler2D texture2;  //FOG OF WAR
uniform sampler2D texture3;  //FOG TEXTURE

uniform vec3 FogColor;
uniform vec4 MapSize;
uniform float FogRepeatSize;
uniform int BorderOnly;

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
  vec4 Color = texture2D( texture0, tc0 );
  vec2 Pos = Unproject().xy;
  //Pos = Pos + vec2(Time*Speed)*Direction;
  //Pos.x = fract(abs(Pos.x)/GridSize);
  //Pos.y = fract(abs(Pos.y)/GridSize);
  float FogValue;       
  if (Pos.x<MapSize.x || Pos.x>MapSize.z || Pos.y<MapSize.y || Pos.y>MapSize.w)
    FogValue = 1.0;
  else{
    if (BorderOnly==1)
	  FogValue = 0.0;
	else{  
      vec2 FogCoord;
	  FogCoord.y = (Pos.x-MapSize.x)/(MapSize.z-MapSize.x);
	  FogCoord.x = (Pos.y-MapSize.y)/(MapSize.w-MapSize.y);
	
      FogValue = texture2D( texture2, FogCoord ).a;
	};
  };
    
  vec3 FogTexture = texture2D( texture3, Pos/vec2(FogRepeatSize) ).xyz*FogColor;
  
  Color = mix(Color,vec4(FogTexture,1.0),FogValue);
  //Color = vec4(1.0);//vec4(vec3(texture2D( texture2, tc0 ).r),1.0);
  gl_FragColor = Color;
}