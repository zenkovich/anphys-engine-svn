#version 150

uniform mat4 ViewProjectionMatrix;
in vec3 Vertex;

void main(void)
{
    gl_Position = ViewProjectionMatrix * vec4(Vertex, 1.0);
}