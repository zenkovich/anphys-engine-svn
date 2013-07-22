varying vec2 tc0;

varying vec2 ScreenPos;

void main(void)
{
vec2 screenPos = sign( gl_Vertex.xy );
ScreenPos = screenPos;

gl_Position = vec4( screenPos, 0.0, 1.0);
tc0 = screenPos * 0.5 + 0.5;
}