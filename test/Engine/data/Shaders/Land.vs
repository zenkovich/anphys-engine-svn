uniform sampler2D heightMap;


uniform float texture1Tile;
uniform float texture2Tile;
uniform float texture3Tile;
uniform float texture4Tile;
uniform float GridSize;
uniform vec3 LightPosition;
uniform vec2 TileShiftPosition;
varying vec4 FragPosition;
varying vec4 VertexPosition;

varying vec3 viewVec;

varying vec2 tc;

varying vec2 texCoord1;
varying vec2 texCoord2;
varying vec2 texCoord3;
varying vec2 texCoord4;

varying vec3 Color;
const float halfTexel = 1.0/264.0;

void main(void)
{
  Color = gl_Color.rgb;
  
  texCoord1 = vec2(gl_Vertex.y/texture1Tile+halfTexel,gl_Vertex.x/texture1Tile+halfTexel);
  texCoord2 = vec2(gl_Vertex.y/texture2Tile+halfTexel,gl_Vertex.x/texture2Tile+halfTexel);
  texCoord3 = vec2(gl_Vertex.y/texture3Tile+halfTexel,gl_Vertex.x/texture3Tile+halfTexel);
  texCoord4 = vec2(gl_Vertex.y/texture4Tile+halfTexel,gl_Vertex.x/texture4Tile+halfTexel);
  

  vec2 texCoord = vec2(gl_Vertex.y/132.0+halfTexel,gl_Vertex.x/132.0+halfTexel);
  tc = texCoord;
  
  vec3 Height = texture2D(heightMap,texCoord).xyz;
  vec4 Vertex = vec4(gl_Vertex.x*GridSize + TileShiftPosition.x, gl_Vertex.y*GridSize+TileShiftPosition.y, Height.x*255.0+Height.y+Height.z*65025.0,1.0);
  vec4 ResultPosition = gl_ModelViewProjectionMatrix*Vertex;
  VertexPosition = Vertex;
  gl_Position = ResultPosition;
  FragPosition = ResultPosition;
}