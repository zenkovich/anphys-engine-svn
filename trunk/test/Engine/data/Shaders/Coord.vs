varying vec3 VertexPos;
varying vec3 EyePos;
varying vec3 VertexModelPos;
varying vec4 glColor;

void main(void)
{
  glColor = gl_Color;
  VertexPos = vec3(gl_Vertex);
  VertexModelPos = vec3(gl_ModelViewMatrix*gl_Vertex);
  EyePos = -vec3(gl_ModelViewMatrix*gl_Vertex);
  EyePos.z = 0.0;
  gl_Position = ftransform();
}