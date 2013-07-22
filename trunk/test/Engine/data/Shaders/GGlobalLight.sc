const float SpecularPower = 2.0;
const float SpecularIntesinity = 0.25;

vec4 directionalLight(vec3 N, vec4 D, vec3 L)
{   
    float nDotL = dot(N, L);	
	  
    if (nDotL > 0.0)
    {   
        return D * nDotL;
    }
   
    return D * 0.0;
}

float SpecularLight(vec3 N, vec3 L)
{
    vec3 reflectVec =  reflect(L,N);
    float spec = max(dot(-reflectVec,viewVec), 0.0); //MinimalSpec
    spec = pow(spec,SpecularPower);
    return 1.0;//spec*SpecularIntesinity;
}