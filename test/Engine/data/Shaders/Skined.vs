#version 120
uniform sampler2D Bones;

varying vec2 texCoord;
uniform vec3 Scale;
	                     
void main(void)
{ 
  float WeightsCount = gl_MultiTexCoord2.z;
  vec4 BoneIDs = gl_MultiTexCoord3;     //texcoord3
  vec4 Weights = gl_MultiTexCoord4;     //texcoord4
  
  mat4 Animation = mat4(0.0);
  for (int weight = 0; weight<WeightsCount; weight++){
    mat4 Bone = transpose(
					 mat4(texture2D(Bones, vec2(0.125,BoneIDs[weight]/128.0+0.0039)),
                     texture2D(Bones, vec2(0.375,BoneIDs[weight]/128.0+0.0039)),
                     texture2D(Bones, vec2(0.625,BoneIDs[weight]/128.0+0.0039)),
                     texture2D(Bones, vec2(0.875,BoneIDs[weight]/128.0+0.0039)))*Weights[weight]
					 );
	Animation = Animation + Bone;
  };
  vec4 Vertex = Animation*gl_Vertex;
  vec3 AnimatedNormal = mat3(Animation)*gl_Normal;
  
  Vertex = Vertex*vec4(Scale,1.0);

  vec4 p = gl_ModelViewMatrix*Vertex;
  gl_ClipVertex = p;
  gl_Position = gl_ModelViewProjectionMatrix*Vertex;
  texCoord = gl_MultiTexCoord2.xy; 
}