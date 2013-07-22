//include "Lights.sh"

uniform sampler2D VolumeLight;
uniform sampler2D OmniShadow0;
uniform sampler2D OmniShadow1;
uniform sampler2D OmniShadow2;
uniform sampler2D OmniShadow3;
  
uniform vec3 LightsPosition[MaxLightsCount];
uniform vec3 LightsColor[MaxLightsCount];
uniform float LightsRadius[MaxLightsCount];
uniform float LightsNoFadePart[MaxLightsCount];
uniform float LightsSpotAngle[MaxLightsCount];
uniform float LightsSpotNoFadeAngle[MaxLightsCount];
uniform vec3 LightsSpotDirection[MaxLightsCount];
uniform int LightsShadowsID[MaxLightsCount];

uniform int LightsID[MaxLightsCount];
uniform int OmniLightsBegin;
uniform int OmniLightsEnd;
uniform int SpotLightsBegin;
uniform int SpotLightsEnd;
            
vec3 getOmniColor(int ID, vec3 Pos, vec3 N, float shadow)
{
  vec3 Result = vec3(0.0,0.0,0.0);  
 	  
  vec3 P = LightsPosition[ID];             
  float MaxDist = LightsRadius[ID];
  float dist = distance(Pos,P);
	  
  if (dist<MaxDist){
    vec3 C = LightsColor[ID];
    float RefDist = LightsNoFadePart[ID];  
	float bright;	
    if (dist<RefDist)
      bright = 1.0;
    else{
      bright =1.0-(dist-RefDist)/(MaxDist-RefDist);
    };
	bright = bright*(1.0 - shadow);
	
	Result = C*bright; 
		
    vec3 lPos = P-Pos;
    lPos = normalize(lPos);  
		
    Result = Result * max(0.0, dot(N,lPos));  
  };     
  //Result = vec3(0.0,1.0,0.0);	  
  return Result;
}

vec3 getSpotColor(int ID, vec3 Pos, vec3 N)
{
  vec3 Result = vec3(0.0,0.0,0.0);  
 	       
  vec3 P = LightsPosition[ID];             
  float MaxDist = LightsRadius[ID];
  float dist = distance(Pos,P);
	  
  if (dist<MaxDist){
    vec3 lPos = P-Pos;
    lPos = normalize(lPos);
    
    vec3 dir = LightsSpotDirection[ID];
    float SpotAngle = LightsSpotAngle[ID];
    float cb = dot(lPos,dir);
    if (cb>SpotAngle){
      vec3 C = LightsColor[ID];
      float RefDist = LightsNoFadePart[ID];       
      if (dist<RefDist)
        Result = C;
      else{
        float r =1.0-(dist-RefDist)/(MaxDist-RefDist);
        Result = C*r; 
      };
		
      //Result = Result * max(0.0, dot(N,lPos));  
    };
  };     
  //Result = vec3(0.0,1.0,0.0);	       
  return Result;
}

vec3 CalcLights(vec3 vPos, vec3 N)
{
	$ifdef NEED_FRAGMENT_POSITION
		vec2 ScreenCoord = (FragPosition.xy/FragPosition.w)*0.5+0.5;
	$endif
	
	float shadows[MaxLightsCount];
	
	$ifdef OMNI_SHADOWS_COUNT_0
		for (int i=0; i<MaxLightsCount; i++)
			shadows[i] = 0.0;
	$else
		$ifdef OMNI_SHADOWS_COUNT_1
			for (int i=1; i<MaxLightsCount; i++)
				shadows[i] = 0.0;
			shadows[0] = texture2D(OmniShadow0,ScreenCoord).x;
		$else
			$ifdef OMNI_SHADOWS_COUNT_2
				for (int i=2; i<MaxLightsCount; i++)
					shadows[i] = 0.0;
				shadows[0] = texture2D(OmniShadow0,ScreenCoord).x;
				shadows[1] = texture2D(OmniShadow1,ScreenCoord).x;
			$else
				$ifdef OMNI_SHADOWS_COUNT_3
					for (int i=3; i<MaxLightsCount; i++)
						shadows[i] = 0.0;
					shadows[0] = texture2D(OmniShadow0,ScreenCoord).x;
					shadows[1] = texture2D(OmniShadow1,ScreenCoord).x;
					shadows[2] = texture2D(OmniShadow2,ScreenCoord).x;
				$else
					$ifdef OMNI_SHADOWS_COUNT_4
						for (int i=4; i<MaxLightsCount; i++)
							shadows[i] = 0.0;
						shadows[0] = texture2D(OmniShadow0,ScreenCoord).x;
						shadows[1] = texture2D(OmniShadow1,ScreenCoord).x;
						shadows[2] = texture2D(OmniShadow2,ScreenCoord).x;
						shadows[3] = texture2D(OmniShadow3,ScreenCoord).x;
					$endif
				$endif
			$endif
		$endif
	$endif
	
	$ifdef VOLUME_LIGHTS
		vec3 vLightColor = texture2D(VolumeLight	,ScreenCoord).xyz;
	$else
		vec3 vLightColor = vec3(0.0);
	$endif
  
// -1+1 - ATI/AMD shader compiler antibug hint.
// if delete -1+1 - optimizer make loop incorrect   
	$ifdef OMNI_LIGHTS
    for (int i=0; i<OmniLightsEnd-1+1; i++)
		vLightColor = vLightColor + getOmniColor(LightsID[i],vPos,N,shadows[i]);
	$endif

	$ifdef SPOT_LIGHTS
	for (int i=OmniLightsEnd-1+1; i<SpotLightsEnd; i++)
		vLightColor = vLightColor + getSpotColor(LightsID[i],vPos,N);
	$endif
//antibug hint      
  
  return vLightColor;
}                         