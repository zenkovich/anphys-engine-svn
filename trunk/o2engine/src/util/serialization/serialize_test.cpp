#include "serialization.h"

OPEN_O2_NAMESPACE

struct cTestSerializeObjA
{
	int         a, b;
	float       c, d;
	std::string e, f;
	char        g[16];

	cTestSerializeObjA()
	{
		a = 1; b = 2;
		c = 3; d = 4;
		e = "e"; f = "fff";
		strcpy(g, "atata");
	}

	SERIALIZE_METHOD_DECL();
};

SERIALIZE_METHOD_IMPL(cTestSerializeObjA)
{
	SERIALIZE(a);
	SERIALIZE(b);
	SERIALIZE(c);
	SERIALIZE(d);
	SERIALIZE(e);
	SERIALIZE(f);
	SERIALIZE_ARR(g, 16);
}

struct cTestSerializeObjB:public cTestSerializeObjA
{
	vec2i       v;
	std::string h;

	cTestSerializeObjB():cTestSerializeObjA()
	{
		v = vec2i(0, 0);
		h = "321";
	}

	SERIALIZE_INHERITED_METHOD_DECL(cTestSerializeObjA, cTestSerializeObjB);
};

SERIALIZE_INHERITED_METHOD_IMPL(cTestSerializeObjB)
{
	SERIALIZE(v);
	SERIALIZE(h);
}

CLOSE_O2_NAMESPACE