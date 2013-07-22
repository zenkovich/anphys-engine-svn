varying vec2 TexCoord;

uniform vec2 Position;

uniform float Size;
uniform float HalfTexel;

varying vec2 mapCoords;
varying vec3 position;
varying vec3 normal;
varying float verticalDistance;

vec2 getMapCoords(){
  float border = HalfTexel* 2.0;
  float scale =  1.0- border*3.0;
  return (gl_Vertex.xy-Position)/Size*scale+vec2(border+HalfTexel);
}

float getVDistance(vec3 Pos, vec3 N){
  vec3 dir = normalize(cross(N,vec3(0.0,0.0,1.0)));
  vec2 d = normalize(dir.xy);
  float Result;
  if (abs(d.x)>abs(d.y))
    Result=Pos.x;
  else	
    Result=Pos.y;
  return Result;
}

void main(void)
{
  TexCoord = gl_MultiTexCoord0.xy;
  gl_Position = ftransform();
  
  vec2 MapCoords = getMapCoords();
  mapCoords = MapCoords;
  
  normal = normalize(gl_Normal);
  position = gl_Vertex.xyz;
  
  verticalDistance = getVDistance(gl_Vertex.xyz, gl_Normal);
}

