#ifndef OBJECTS_H
#define OBJECTS_H

#include "public.h"

OPEN_O2_NAMESPACE

/** Basic drawable object interface. */
class IDrawable
{
public:
	/** Draw operations. */
	virtual void draw() = 0;
};

/** Basic dynamic object interface. */
class IDynamic
{
public:
	/** Update condition in delta time. */
	virtual void update(float dt) = 0;
};

/** Basic dynamic drawable object. */
class IDynamicDrawable :public virtual IDynamic, public virtual IDrawable {};

CLOSE_O2_NAMESPACE

#endif //OBJECTS_H