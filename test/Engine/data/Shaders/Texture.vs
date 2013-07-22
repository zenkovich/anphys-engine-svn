varying vec2 tc0;

void main(void)
{
vec2 screenPos = gl_Vertex.xy;//= sign( gl_Vertex.xy );

gl_Position = vec4( screenPos, 0.0, 1.0);
tc0 = screenPos * 0.5 + 0.5;
}