uniform vec4 FogColor;
uniform float FogStart;
uniform float FogEnd;
uniform float FogScale;
uniform int   FogEnabled;

uniform float Animation; 
uniform sampler2D texture1;
uniform sampler2D texture2;

varying vec2 TexCoord;

uniform vec3 EyePosition; 
varying vec3 VertexPosition;

void main(void)
{
   float Distance = distance(VertexPosition,EyePosition);
   float MaxDist = 1500.0;
   vec4 Color1 = texture2D(texture2, TexCoord);
   vec4 Color2 = texture2D(texture1, TexCoord);
   vec4 Color = vec4((Color1.x*Animation)+(Color2.x*(1.0-Animation)));

   float fog = 0.0;
   if (FogEnabled!=0)
   {
       fog = clamp((FogEnd- gl_FogFragCoord)*FogScale,0.0,1.0);
   };
   if (Distance>MaxDist || fog>0.99)
     Color = vec4(0.5,0.5,0.5,1.0);
   else
     Color = vec4(0.5,0.5,0.5,1.0)*(Distance/MaxDist)+Color*(1.0-Distance/MaxDist);
   //Color = vec4(Distance/MaxDist,0.0,0.0,1.0);  
   gl_FragColor = vec4(Color.x,Color.y,1.0,Color.w);
}