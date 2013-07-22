$ifdef LIGHTMAP
uniform sampler2D tex_LightMap;
in vec2 lightMapTexCoord;
vec3 lightCalc()
{
	return texture2D(tex_LightMap, lightMapTexCoord).xyz;
}
$else
vec3 lightCalc()
{
}
$endif