varying vec4 glColor;
varying vec2 texCoord;
	                     
void main(void)
{
	vec4 p = gl_ModelViewMatrix*gl_Vertex;
	gl_ClipVertex = p;
	glColor = gl_Color;
	texCoord = gl_MultiTexCoord0.xy; 
	gl_Position = ftransform();
}