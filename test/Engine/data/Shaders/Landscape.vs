#version 120
//include "Default_Constants.sh"
varying vec3 viewVec;

varying vec2 texCoord1;
varying vec2 texCoord2;
varying vec2 texCoord3;

varying vec2 FilterTexCoord;
varying vec4 glColor;

varying vec3 envTexCoord;

varying vec4 VertexPosition;
varying vec4 VertexEyePosition;

varying vec4 FragPosition;

uniform vec3 LightPosition;
varying vec3 locLight;
varying float Light;

varying vec3 Normal;

uniform mat4x4 WorldMatrix;

uniform vec4 BBSize;
varying vec2 BBCoord;
	                     
void main(void)
{
  vec4 p = gl_ModelViewMatrix*gl_Vertex;
  gl_ClipVertex = p;
  glColor = gl_Color;
  vec4 ecPos = gl_ModelViewMatrix*gl_Vertex; 
  vec4 ResultPosition = gl_ProjectionMatrix * ecPos;
  FragPosition = ResultPosition;
  gl_Position = ResultPosition;
  texCoord1 = gl_MultiTexCoord0.xy; 
  texCoord2 = gl_MultiTexCoord1.xy; 
  texCoord3 = gl_MultiTexCoord2.xy; 
  FilterTexCoord = gl_MultiTexCoord3.xy;

  vec3 n = normalize(gl_NormalMatrix * gl_Normal);
  vec3 t = normalize(gl_NormalMatrix * gl_MultiTexCoord4.xyz);
  vec3 b = normalize(gl_NormalMatrix * gl_MultiTexCoord5.xyz);
  
  Normal = n;
		
  vec3 v;
  vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
  VertexEyePosition = ecPos;
	
  vec3 vVec = vVertex;
  v.x = dot(vVec, t);
  v.y = dot(vVec, b);
  v.z = dot(vVec, n);
  viewVec = v;
  gl_FogFragCoord = abs(ecPos.z);
  VertexPosition =  gl_Vertex;
  
  BBCoord = vec2((VertexPosition.x-BBSize.x)/BBSize.z,(VertexPosition.y-BBSize.y)/BBSize.w);
  
   vec3 reyeVec = reflect(vVertex,n);
   vec3 objREV = (gl_ModelViewMatrixInverse*vec4(reyeVec,0.0)).xyz;
   envTexCoord = normalize(objREV.xyz);     
   
   vec3 localLight = normalize(gl_NormalMatrix*LightPosition);
   locLight = localLight;   
   Light = max(0.2,dot(normalize(mat3(WorldMatrix)*gl_Normal),localLight)); 
}