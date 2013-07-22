
varying vec2 texCoord;
varying vec4 glColor;
   
void main(void)
{
  vec4 p = gl_ModelViewMatrix*gl_Vertex;
  gl_ClipVertex = p;
  glColor = gl_Color;
  vec4 ecPos = p; 
  gl_Position = ftransform();
  texCoord = gl_MultiTexCoord0.xy; 
  
  gl_FogFragCoord = abs(ecPos.z);
}