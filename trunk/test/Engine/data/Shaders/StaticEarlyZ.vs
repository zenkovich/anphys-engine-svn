#version 120
void main(void)
{
	vec4 p = gl_ModelViewMatrix*gl_Vertex;
	gl_ClipVertex = p;
	gl_Position = gl_ProjectionMatrix * p; 
}