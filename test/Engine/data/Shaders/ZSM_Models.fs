uniform sampler2D Textures[4];
uniform sampler2D MixMap;
uniform sampler2D ColorMap;
uniform vec2 TextureSize[4];
uniform int IsTextureVertical[4];
uniform float Enabled;

varying vec2 mapCoords;
varying vec3 position;
varying vec3 normal;
varying float verticalDistance;

uniform sampler2D Texture;
varying vec2 TexCoord;

uniform vec3 Sun;

vec2 getTexCoord(int id){
  vec2 Result;
  if (IsTextureVertical[id]==1)
    Result = vec2(fract(verticalDistance/TextureSize[id].x),1.0-fract(position.z/TextureSize[id].y));
  else	
    Result = vec2(fract(position.x/TextureSize[id].x),fract(position.y/TextureSize[id].y));
  return Result;
}

vec3 getTexturesMix(vec3 Mix){
  vec3 Result = vec3(0.0);
  if (Mix.x>0.0)
	Result += texture2D(Textures[0], getTexCoord(0)).xyz* Mix.x;
  if (Mix.y>0.0)
	Result += texture2D(Textures[1], getTexCoord(1)).xyz* Mix.y;
  if (Mix.z>0.0)
	Result += texture2D(Textures[2], getTexCoord(2)).xyz* Mix.z;	
  if (Mix.x+Mix.y+Mix.z<1.0)
	Result += texture2D(Textures[3], getTexCoord(3)).xyz* (1.0-Mix.x-Mix.y-Mix.z);	
  return Result;
}

float getLight(){
  vec3 N;
  N = normalize(normal);
  return max(0.2,dot(N,Sun));
}

void main(void)
{
  vec4 Result = texture2D(Texture,TexCoord);
  
  vec4 Mix = texture2D(MixMap, mapCoords);  
  float LandMix = Mix.w*Result.w;
  if (LandMix<1.0){
    vec3 Mix= getTexturesMix(Mix.xyz);  
    vec4 Color = texture2D(ColorMap, mapCoords);
    Mix = Mix*Color.xyz*vec3(Color.w);
	Result.xyz = mix(Mix,Result.xyz, LandMix);
  }
  
  Result.xyz = Result.xyz*vec3(getLight());
  
  gl_FragColor = vec4(mix(vec3(1.0,0.0,0.0),Result.xyz,Enabled),1.0);
}
