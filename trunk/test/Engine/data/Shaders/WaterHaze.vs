varying vec2 TexCoord; 
varying vec3 VertexPosition;

void main(void) 
{ 
  TexCoord = vec2(gl_MultiTexCoord0); 
  gl_Position = ftransform(); 
  VertexPosition = gl_Vertex.xyz;
}