uniform sampler2D texture;

varying vec2 TexCoord;
varying vec4 glColor;

void main(void)
{
   //TODO: FOG
   vec4 Color = texture2D(texture, TexCoord);
   gl_FragColor = Color*glColor;
}