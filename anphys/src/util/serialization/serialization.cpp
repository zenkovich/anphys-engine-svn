#include "serialization.h"

#include "data_object.h"

//test
struct sstt
{
	float a, b, c;
	unsigned int d, e, f;

	sstt():a(0), b(1), c(2), d(3), e(4), f(4) {}

	serializeMethodDecl(sstt)
	{
		autoSerialize(a);
		autoSerialize(b);
		autoSerialize(c);
		autoSerialize(d);
		autoSerialize(e);
		autoSerialize(f);

		return true;
	}
};

struct SomeDataObjectTestStruct
{
	float obj, b, c;
	std::string id;

	sstt st;

	SomeDataObjectTestStruct():obj(66.3f), b(0), c(0), id("some good string")
	{
	}

	serializeMethodDecl(SomeDataObjectTestStruct)
	{
		autoSerialize(obj);
		autoSerialize(b);
		autoSerialize(c);

		serializeId(id, "atata");

		autoSerializeObj(st);

		return true;
	}
};

struct DerivedTestStruct:public SomeDataObjectTestStruct
{
	std::string idp;

	DerivedTestStruct():SomeDataObjectTestStruct(), idp("preved") {}

	derivedSerializeMethodDecl(SomeDataObjectTestStruct, DerivedTestStruct)
	{
		autoSerialize(idp);

		return true;
	}
};

void testDataObject()
{
	cDataObject root("BORODA");

	//fill some data to root
	root.addChild(3.44f, "a");
	root.addChild("some string", "b")->addChild(true, "c");

	//serialize test
	SomeDataObjectTestStruct testSerializeStruct;
	testSerializeStruct.serialize(root, AT_OUTPUT, "TestSerializeObject");

	DerivedTestStruct derivedTestStruct;
	derivedTestStruct.serialize(root, AT_OUTPUT, "DerivedTestSerializeObject");

	//getting data test
	float a = root.getChildValue<float>("a");
	cDataObject* aObj = root.getChild("a");

	std::string b = root.getChildValue<std::string>("b");
	cDataObjectContainer<std::string>* bObjContainer = root.getChild<std::string>("b");

	bool c = bObjContainer->getChildValue<bool>("c");

	bool cPath = root.getChildValue<bool>("b/c");

	//copy root
	cDataObject rootCopy = root;

	//testing save/load
	char testFile[64] = "test_data_object.xml";
	char testFileRest[64] = "test_data_object_2.xml";

	//save
	pugi::xml_document doc;
	root.saveDataTo(doc.append_child());
	doc.save_file(testFile);
	doc.reset();

	//restore from saved
	cDataObject restoreRoot;
	SomeDataObjectTestStruct restoreTestSerializeStruct;
	DerivedTestStruct restoredDerivedTestStruct;

	restoreTestSerializeStruct.id = "other string";
	restoredDerivedTestStruct.id = "wrong string";

	doc.load_file(testFile);
	restoreRoot.loadDataFrom(doc.first_child());
	doc.reset();

	restoreTestSerializeStruct.serialize(restoreRoot, AT_INPUT, "TestSerializeObject");
	restoredDerivedTestStruct.serialize(restoreRoot, AT_INPUT, "DerivedTestSerializeObject");

	//save restored
	pugi::xml_document doc2;
	restoreRoot.saveDataTo(doc2.append_child());
	doc2.save_file(testFileRest);
	doc2.reset();
}