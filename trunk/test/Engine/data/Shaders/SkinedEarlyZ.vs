#version 120

uniform sampler2D Bones;
uniform vec3 Scale;
//attribute vec4 BoneIDs;     //texcoord1
//attribute vec4 Weights;     //texcoord2
//attribute float WeightsCount;//texcoord0.z
	                     
void main(void)
{ 
  float WeightsCount = gl_MultiTexCoord0.z;
  vec4 BoneIDs = gl_MultiTexCoord1;     //texcoord1
  vec4 Weights = gl_MultiTexCoord2;     //texcoord2
  
  vec4 Vertex = vec4(0.0);
  for (int weight = 0; weight<WeightsCount; weight++){
    mat4 Bone = transpose(
					 mat4(texture2D(Bones, vec2(0.125,BoneIDs[weight]/128.0+0.0039)),
                     texture2D(Bones, vec2(0.375,BoneIDs[weight]/128.0+0.0039)),
                     texture2D(Bones, vec2(0.625,BoneIDs[weight]/128.0+0.0039)),
                     texture2D(Bones, vec2(0.875,BoneIDs[weight]/128.0+0.0039)))*Weights[weight]
					 );
	Vertex = Vertex + Bone*gl_Vertex;
  };
  Vertex = Vertex*vec4(Scale,1.0);
  vec4 p = gl_ModelViewMatrix*Vertex;
  gl_ClipVertex = p;
  gl_Position = gl_ModelViewProjectionMatrix*Vertex;
}