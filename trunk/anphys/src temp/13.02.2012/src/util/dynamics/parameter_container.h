#ifndef PARAMETER_CONTAINER_H
#define PARAMETER_CONTAINER_H


/** Контейнер на какою-либо переменную. Хранит в себе ее указатель. */
template<typename object> struct cParameterContainer
{
	object* mObjectPointer; /**< Указатель на хранимую переменную. */

	inline cParameterContainer() { mObjectPointer = NULL; }
	inline cParameterContainer(object* objectPointer) { mObjectPointer = objectPointer; } 
	inline ~cParameterContainer() {}

	inline void setValue(object value) { *mObjectPointer = value; }
	inline object getValue() { return *mObjectPointer; }
	inline void setPtr(object* objectPointer) { mObjectPointer = objectPointer; }
	inline object* getPtr() { return mObjectPointer; }
		
	inline void operator=(object& obj) { *mObjectPointer = obj; }
	inline void operator=(cParameterContainer<object>& container) { *mObjectPointer = *container.mObjectPointer; }
};

#endif //PARAMETER_CONTAINER_H
