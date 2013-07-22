uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D normalMap;
uniform sampler2D colorsMap;
uniform sampler2D colorMap;

uniform vec3 LightPosition;
varying vec4 FragPosition;
varying vec4 VertexPosition;

varying vec2 tc;

varying vec2 texCoord1;
varying vec2 texCoord2;
varying vec2 texCoord3;
varying vec2 texCoord4;

varying vec3 Normals;
varying vec3 Color;

//include "GPointLights_fs.sc"
//include "GDayLight_fs.sc"
//include "GFog_fs.sc"    

void main(void)
{
  vec3 Normal = texture2D(normalMap,tc).xyz;
  Normal = Normal - vec3(0.5,0.5,0.5);
  Normal = normalize(Normal);
  vec3 PointLights = CalcLights(VertexPosition.xyz,Normal);
  
  float Light = max(0.2,dot(Normal,LightPosition));  //));
  vec4 FullLight = vec4(MixWithDayLight(PointLights,Light),1.0);
  
  vec4 Colors = texture2D(colorsMap,tc);

  vec4 Result = vec4(0.0,0.0,0.0,0.0);
  if (Colors.x>0)
    Result = texture2D(texture1, texCoord1)*Colors.x;
  if (Colors.y>0)	
    Result = Result+texture2D(texture2, texCoord2)*Colors.y;
  if (Colors.z>0)	
    Result = Result+texture2D(texture3, texCoord3)*Colors.z;
  if (Colors.w>0)	
    Result = Result+texture2D(texture4, texCoord4)*Colors.w;
    
  Result = Result * texture2D(colorMap,tc);  	
  Result = vec4(Color,1.0)*Result;
  Result = MixWithFog(Result);
  //Result = Normals;
  //TODO Fog, Light

  gl_FragColor = vec4(Result.xyz*FullLight.xyz,1.0);
}