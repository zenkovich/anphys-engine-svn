vec3 NormalTransform(vec3 p, vec4 rot)
{
	// original code from DooM 3 SDK
    float xxzz = rot.x*rot.x - rot.z*rot.z;
    float wwyy = rot.w*rot.w - rot.y*rot.y;
    float xw2 = rot.x*rot.w*2.0;
    float xy2 = rot.x*rot.y*2.0;
	
    float xz2 = rot.x*rot.z*2.0;
    float yw2 = rot.y*rot.w*2.0;
    float yz2 = rot.y*rot.z*2.0;
    float zw2 = rot.z*rot.w*2.0;
	
    vec3 ret = vec3((xxzz + wwyy)*p.x + (xy2 + zw2)*p.y       + (xz2 - yw2)*p.z,
                    (xy2 - zw2)*p.x   + (y*y+w*w-x*x-z*z)*p.y + (yz2 + xw2)*p.z,
                    (xz2 + yw2)*p.x   + (yz2 - xw2)*p.y       + (wwyy - xxzz)*p.z);
	return ret;
}

vec3 VertexTransform(vec3 p, vec4 rot, vec3 pos)
{
    // original code from DooM 3 SDK
    float xxzz = rot.x*rot.x - rot.z*rot.z;
    float wwyy = rot.w*rot.w - rot.y*rot.y;
    float xw2 = rot.x*rot.w*2.0;
    float xy2 = rot.x*rot.y*2.0;
	
    float xz2 = rot.x*rot.z*2.0;
    float yw2 = rot.y*rot.w*2.0;
    float yz2 = rot.y*rot.z*2.0;
    float zw2 = rot.z*rot.w*2.0;
	
    vec3 ret = vec3((xxzz + wwyy)*p.x + (xy2 + zw2)*p.y       + (xz2 - yw2)*p.z,
                    (xy2 - zw2)*p.x   + (y*y+w*w-x*x-z*z)*p.y + (yz2 + xw2)*p.z,
                    (xz2 + yw2)*p.x   + (yz2 - xw2)*p.y       + (wwyy - xxzz)*p.z);
 
    return ret + pos;
}