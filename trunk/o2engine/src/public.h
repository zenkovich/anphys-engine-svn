#ifndef PUBLIC_H
#define PUBLIC_H

//namespace macros
#define OPEN_O2_NAMESPACE namespace o2 {
#define CLOSE_O2_NAMESPACE }

//safe release objects
#define safe_release(obj) { if (obj != 0) delete obj; }
#define safe_release_arr(obj) { if (obj != 0) delete[] obj; }

#endif //PUBLIC_H