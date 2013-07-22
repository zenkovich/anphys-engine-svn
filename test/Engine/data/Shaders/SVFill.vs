varying vec3 Color;
void main(void)
{
  Color = gl_Color.xyz;
  vec2 screenPos = sign( gl_Vertex.xy );
  gl_Position = vec4( screenPos, 0.0, 1.0);
}