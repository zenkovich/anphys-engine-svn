$ifdef LIGHTMAP
void lightCalc()
{
}
$else
in vec3 Normal;
in vec3 Tangent;
in vec3 Binormal;
in mat4 WorldMatrix;

out vec3 modelVec;

void lightCalc()
{
    vec3 v;
    v.x = dot(Vertex, Normal);
    v.y = dot(Vertex, Tangent);
    v.z = dot(Vertex, Binormal);
    modelVec = Vertex;
}
$endif