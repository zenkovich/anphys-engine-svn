//include "Default_Constants.sh"
varying vec3 viewVec;

varying vec2 texCoord1;
varying vec2 texCoord2;
varying vec2 texCoord3;

varying vec3 envTexCoord;

varying vec2 FilterTexCoord;
varying vec4 glColor;

varying vec3 locLight;
varying vec3 Normal;
varying float Light;

uniform sampler2D tex_Color1;
uniform sampler2D tex_Color2;
uniform sampler2D tex_Color3;
uniform sampler2D tex_Filter;
uniform sampler2D tex_Composite;

uniform samplerCube tex_Env;

uniform sampler2D BBTex;
varying vec2 BBCoord;

varying vec4 VertexPosition;
varying vec4 VertexEyePosition;
uniform vec4 EyePosition;

varying vec4 FragPosition;

//include "GDayLight_fs.sc"
//include "GFog_fs.sc"
//include "GGlobalLight.sc"
//include "GPointLights_fs.sc"
	
vec3 getTexValue(sampler2D tex, vec2 coord, vec2 view)
{
    vec4 c = texture2D(tex, coord);
	if (c.w>0.01 && c.w<1.0)
	  c = texture2D(tex, coord + ((c.w * 0.04 - 0.02) * (-view)) );

    return c.xyz;
}
	
void main (void)
{
    vec4 ResultColor;
    vec3 PointLights = CalcLights(VertexPosition.xyz,Normal);
    vec3 vVec = normalize(viewVec);

	vec3 base1 = vec3(0.0);
	vec3 base2 = vec3(0.0);
	vec3 base3 = vec3(0.0);
	
	vec4 filter = texture2D(tex_Filter, FilterTexCoord);
	if (filter.x>0.0) base1 = getTexValue(tex_Color1, texCoord1, vVec.xy);
	if (filter.y>0.0) base2 = getTexValue(tex_Color2, texCoord2, vVec.xy);
	if (filter.z>0.0) base3 = getTexValue(tex_Color3, texCoord3, vVec.xy);
	
    vec4 base = vec4((base1*filter.x+base2*filter.y+base3*filter.z)*filter.w,1.0);
	
	vec4 Composite = texture2D(tex_Composite, FilterTexCoord);
	if (Composite.g>0.0)
	  base.xyz = mix(base.xyz,textureCube(tex_Env, envTexCoord).xyz,Composite.g);
	  
	vec4 FullLight = vec4(MixWithDayLight(PointLights,Light)+0.2*vec3(Composite.a)*SpecularLight(Normal,locLight),1.0);  
	
       
    ResultColor = vec4(base.xyz*glColor.xyz,1.0);
    ResultColor = MixWithFog(ResultColor);
	vec4 BBColor = texture2D(BBTex,BBCoord);
	ResultColor = vec4(mix(ResultColor.xyz,BBColor.xyz,BBColor.w),ResultColor.w);
	
	FullLight.w = ResultColor.w;
	ResultColor.w = glColor.w*base.w;
    
    //ResultColor = vec4(Normal/2.0+vec3(0.5,0.5,0.5),1.0);
  gl_FragColor = vec4(ResultColor.xyz*FullLight.xyz,ResultColor.w);
  //gl_FragData[1] = FullLight;
}