#extension EXT_gpu_shader4 : enable

uniform vec4 LightPosition;
uniform float ShadowLength;


void DetectAndProcessSilhouette(vec3 v1, vec3 v2, vec3 extrude1,vec3 extrude2) 
{ 
    vec3 outpos[4];

    outpos[0] = v1 + ShadowLength*extrude1; 
    outpos[1] = v1; 
    outpos[2] = v2 + ShadowLength*extrude2; 
    outpos[3] = v2;

    // Extrude silhouette and create 2 triangles(trinagle strip)
    for(int v=0; v<4; v++) 
    { 
        gl_Position = gl_ProjectionMatrix * vec4(outpos[v],1.0) ; 
        EmitVertex();
    } 
    EndPrimitive();
}

void main(void)
{
    //Check, is trinagle in light zone
	float dist[3];
	dist[0] = distance(LightPosition.xyz,gl_PositionIn[0].xyz);
	dist[1] = distance(LightPosition.xyz,gl_PositionIn[1].xyz);
	dist[2] = distance(LightPosition.xyz,gl_PositionIn[2].xyz);
    if (dist[0]<ShadowLength || dist[1]<ShadowLength || dist[2]<ShadowLength){
		// Calc unnormalize triangle normal vector
		vec3 N = cross( gl_PositionIn[1].xyz - gl_PositionIn[0].xyz, gl_PositionIn[2].xyz - gl_PositionIn[0].xyz ); 
		
		// If triagnle cast shadow
		if( dot(N, normalize(LightPosition.xyz-gl_PositionIn[1].xyz)) < 0.0 ) 
		{ 
			vec3 lightDir[3];
			lightDir[0] = normalize(gl_PositionIn[0].xyz-LightPosition.xyz);
			lightDir[1] = normalize(gl_PositionIn[1].xyz-LightPosition.xyz);
			lightDir[2] = normalize(gl_PositionIn[2].xyz-LightPosition.xyz);
		
			// Extrude silhouette
			DetectAndProcessSilhouette( gl_PositionIn[1].xyz, gl_PositionIn[0].xyz,lightDir[1],lightDir[0]); 
			DetectAndProcessSilhouette( gl_PositionIn[2].xyz, gl_PositionIn[1].xyz,lightDir[2],lightDir[1]); 
			DetectAndProcessSilhouette( gl_PositionIn[0].xyz, gl_PositionIn[2].xyz,lightDir[0],lightDir[2]);

			// far plane
			int v;
			
			for(v=0; v<3; v++)  
			{ 
				vec3 pos = gl_PositionIn[v].xyz + ShadowLength*lightDir[v]; 
				gl_Position = gl_ProjectionMatrix * vec4(pos,1); 
				EmitVertex(); 
			} 
			EndPrimitive();

			// near plane (reverse order) 
			
			for(v=2; v>=0; v--)
			{ 
				vec3 pos = gl_PositionIn[v].xyz; 
				gl_Position =  gl_ProjectionMatrix * vec4(pos,1); 
				EmitVertex(); 
			} 
			EndPrimitive();
		} 
	}
}