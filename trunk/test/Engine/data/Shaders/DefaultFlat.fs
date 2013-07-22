uniform sampler2D texture;
varying vec2 tc0;

void main(void)
{
gl_FragColor = texture2D( texture, tc0, 1.0 );
}