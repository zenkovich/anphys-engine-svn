uniform sampler2D tex_SphereMap;

vec4 FakeReflection(vec3 _EyePos, vec3 _Normal){
    float m;
	vec3 r,u;
	u = normalize(_EyePos);
	r = reflect(u, _Normal);
	m = 2.0 * sqrt(r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0));
	return texture2D(tex_SphereMap,vec2(r.x/m + 0.5,r.y/m + 0.5));
}