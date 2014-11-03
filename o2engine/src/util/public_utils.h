#ifndef PUBLIC_UTILS_H 
#define PUBLIC_UTILS_H

#include "public_namespace.h"

OPEN_O2_NAMESPACE

/** Converts expr to string. */
#define asString(expr) #expr

/** Enable assets saving protection macros. */
#define ENABLE_EDITING_ASSETS_PROTECTION \
#if ENABLE_EDITING_ASSETS != true \
protected: \
#endif 

CLOSE_O2_NAMESPACE

#endif //PUBLIC_UTILS_H
