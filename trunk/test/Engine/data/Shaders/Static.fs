//include "Default_Constants.sh"
//include "GFakeReflect_fs.sc"
varying vec3 viewVec;
varying vec2 texCoord;
varying vec2 LightMaptexCoord;
varying vec4 glColor;
varying vec3 envTexCoord;

varying vec4 VertexPosition;
varying vec3 Normal;
varying vec3 locLight;
varying vec4 FragPosition;
uniform vec4 EyePosition;

uniform vec3 LightPosition;

$ifndef NORMAL_MAP
varying float Light;
$endif

varying mat3 TriangleSpace;


uniform sampler2D tex_Diffuse;
uniform sampler2D tex_Composite;
uniform sampler2D tex_LightMap;
uniform sampler2D tex_Filter;
uniform sampler2D tex_NormalMap;
uniform samplerCube tex_Env;

//include "GPointLights_fs.sc"
//include "GDayLight_fs.sc"
//include "GFog_fs.sc"
//include "GGlobalLight.sc"                    


void main (void)
{
	vec4 ResultColor;
	
	vec3 N;
    $ifdef NORMAL_MAP
      N = normalize( TriangleSpace * (texture2D(tex_NormalMap, texCoord).xyz * 2.0 - 1.0));
	  float Light = max(0.0,dot(N,LightPosition)); 
    $else
      N = Normal;
    $endif
	
	//Calculate local lights
	$ifndef NO_LOCAL_LIGHTS
		vec3 PointLights = CalcLights(VertexPosition.xyz,N);
	$else
		vec3 PointLights = vec3(0.0);
	$endif
	
	//Calculate paralax
	vec2 newTexCoord = texCoord;
	$ifdef COMPOSITE_MAP
		vec3 vVec = normalize(viewVec);
		float height = texture2D(tex_Composite, texCoord).z ;
		if (height>0.01)
			newTexCoord = texCoord + ((height * 0.04 - 0.02) * (-vVec.xy));
	$endif

    vec4 base = texture2D(tex_Diffuse, newTexCoord);	
	   
	$ifdef COMPOSITE_MAP
		vec4 Composite = texture2D(tex_Composite, newTexCoord);
		float selfilum = Composite.r;
		if (Composite.g>0.0)
			base.xyz = mix(base.xyz,textureCube(tex_Env, envTexCoord).xyz,Composite.g);
		vec4 FullLight = vec4(MixWithDayLight(PointLights,Light)+vec3(selfilum)+0.2*vec3(Composite.a)*SpecularLight(N,locLight),1.0);
	$else
		vec4 FullLight = vec4(MixWithDayLight(PointLights,Light),1.0);
	$endif
	
	ResultColor = vec4(base.xyz*glColor.xyz,1.0);
	$ifdef LIGHT_MAP
		vec4 lightmap = vec4(texture2D(tex_LightMap, LightMaptexCoord).xyz,1.0);
		ResultColor = vec4(ResultColor.xyz*lightmap.xyz,1.0);
	$endif
	
	$ifdef FOG
		ResultColor = MixWithFog(ResultColor);
	$endif
	
	FullLight.w = ResultColor.w;
	ResultColor.w = glColor.w*base.w;
    
    gl_FragColor = vec4(ResultColor.xyz*FullLight.xyz,ResultColor.w);
}