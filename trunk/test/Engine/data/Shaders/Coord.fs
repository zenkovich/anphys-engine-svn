varying vec3 VertexPos;
varying vec3 VertexModelPos;
varying vec3 EyePos;
varying vec4 glColor;

//include "Coord_Constants.sh"

float md(float v1, float v2)
{
  float v = floor(v1/v2);
  return (v1-v*v2);
}

void main(void)
{
  float dist = distance(EyePos,VertexModelPos);
  vec4 emptycolor = vec4(0.0,0.0,0.0,0.0);
  vec4 fillcolor = vec4(glColor.xyz,1.0-1.0/(100.0/dist));
  vec4 color = emptycolor;
  
  
  if (dist<100.0 && (fract(abs(VertexPos.x))<Width*(1.0-fillcolor.w) || fract(abs(VertexPos.y))<Width*(1.0-fillcolor.w)))
    color = fillcolor;
  gl_FragColor = color;
}