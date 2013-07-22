varying vec2 tc0;

uniform float exposure;
uniform float brightMax;

varying float factor;

void main(void)
{
vec2 screenPos = sign( gl_Vertex.xy );

gl_Position = vec4( screenPos, 0.0, 1.0);
tc0 = screenPos * 0.5 + 0.5;
}