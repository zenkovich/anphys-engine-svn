//fog must be place in $ifdef block in main shader

uniform vec4 FogColor;
uniform float FogStart;
uniform float FogEnd;
uniform float FogScale;
uniform int   FogEnabled;

vec4 MixWithFog(vec4 Color){
	vec4 Result = Color;
	if (FogEnabled!=0)
	{
		float fog = clamp((FogEnd- gl_FogFragCoord)*FogScale,0.0,1.0);
		Result = vec4(mix(Color.xyz,FogColor.xyz,(1.0-fog)*FogColor.w),fog*FogColor.w*Color.w);
	} 
	return Result;
}