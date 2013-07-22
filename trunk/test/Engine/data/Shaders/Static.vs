#version 120
//include "Default_Constants.sh"
varying vec3 viewVec; //parallax
varying vec2 texCoord; 
varying vec2 LightMaptexCoord; //lightmap
varying vec4 glColor;
varying vec3 Normal;  //local lights
varying vec3 envTexCoord;  //reflections

varying vec3 locLight;  //global specular
$ifndef NORMAL_MAP
varying float Light;    //global light
$endif
varying vec4 FragPosition; //local lights

uniform mat4x4 WorldMatrix;
uniform vec3 LightPosition;
varying vec4 VertexPosition; //local lights

varying mat3 TriangleSpace;
	                     
void main(void)
{
	glColor = gl_Color;
	
	vec4 VertexInCameraSpace = gl_ModelViewMatrix*gl_Vertex;
	gl_ClipVertex = VertexInCameraSpace;
	
	vec3 WorldSpaceNormal = normalize(mat3(WorldMatrix) * gl_Normal); //Convert normal to world space
	Normal = WorldSpaceNormal;
	vec4 ResultPosition = gl_ProjectionMatrix * VertexInCameraSpace;
	gl_Position = ResultPosition;
	FragPosition = ResultPosition;
	
	texCoord = gl_MultiTexCoord0.xy; 
	$ifdef LIGHT_MAP
		LightMaptexCoord = gl_MultiTexCoord1.xy; 
	$endif
	
   
    locLight = LightPosition;
	$ifndef NORMAL_MAP
    Light = max(0.0,dot(WorldSpaceNormal,LightPosition)); 
	$endif

    vec3 n = WorldSpaceNormal;
    vec3 t = normalize(mat3(WorldMatrix) * gl_MultiTexCoord2.xyz);
    vec3 b = normalize(mat3(WorldMatrix) * gl_MultiTexCoord3.xyz);
	
	TriangleSpace = mat3(t,b,n);
		
    vec3 v;
    v.x = dot(VertexInCameraSpace.xyz, t);
    v.y = dot(VertexInCameraSpace.xyz, b);
    v.z = dot(VertexInCameraSpace.xyz, n);
    viewVec = v;
	gl_FogFragCoord = abs(VertexInCameraSpace.z);
	VertexPosition =  WorldMatrix*gl_Vertex;
  
   vec3 reyeVec = reflect(VertexInCameraSpace.xyz,Normal);
   vec3 objREV = (gl_ModelViewMatrixInverse*vec4(reyeVec,0.0)).xyz;
   envTexCoord = normalize(objREV.xyz);    
}