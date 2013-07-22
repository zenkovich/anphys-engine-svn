uniform vec3 DayLightColor;
uniform vec3 AmbientColor;


vec3 MixWithDayLight(vec3 vLightColor, float Light){
    vec3 Result = AmbientColor + vLightColor.xyz;
	$ifdef GLOBAL_LIGHT
		Result = Result + DayLightColor*vec3(Light);
	$endif
    return Result;
}