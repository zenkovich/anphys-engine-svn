#version 150

uniform mat4 ViewProjectionMatrix;
in vec3 Vertex;
in vec3 TexCoord;

out vec2 texCoord;

//include "lightCalc_vs.sc"

void main(void)
{
	texCoord = TexCoord.xy;
    gl_Position = ViewProjectionMatrix * vec4(Vertex, 1.0);
	lightCalc();
}