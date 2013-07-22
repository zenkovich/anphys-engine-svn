varying vec2 TexCoord; 
varying vec4 glColor;

void main(void) 
{ 
  glColor = gl_Color;
  TexCoord = vec2(gl_MultiTexCoord0); 
  gl_Position = ftransform(); 
}