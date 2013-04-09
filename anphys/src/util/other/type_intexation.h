#ifndef TYPE_INDEXATION_H
#define TYPE_INDEXATION_H

#define UniqueType unsigned int
struct TypeIdxContainer
{
	static UniqueType index;
};

#define DEFINE_TYPE(TYPENAME) public:                              \
	static UniqueType TYPENAME ## Type;                            \
	static UniqueType getStaticType() { return TYPENAME ## Type; } \
	virtual UniqueType getType() { return TYPENAME ## Type; }      \
	virtual const char* getTypeName() { return #TYPENAME; }

#define REGIST_TYPE(TYPENAME) UniqueType TYPENAME::TYPENAME ## Type = TypeIdxContainer::index++;


#endif //TYPE_INDEXATION_H