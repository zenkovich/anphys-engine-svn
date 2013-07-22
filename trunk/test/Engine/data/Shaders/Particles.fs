varying vec4 glColor;
varying vec2 texCoord;

uniform sampler2D texture;

void main (void)
{
    vec4 color = texture2D(texture, texCoord) * glColor;
    gl_FragColor = color;
}