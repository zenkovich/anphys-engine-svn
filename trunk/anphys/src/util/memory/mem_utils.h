#ifndef MEM_UTILS_H
#define MEM_UTILS_H

#define safe_release(obj) if (obj != NULL) delete obj;
#define safe_release_arr(arr) if (arr != NULL) delete[] arr;

#define copyCycle(dest, desc, count) for (int i = 0; i < count; i++) dest[i] = desc[i];
#define defineArray(arr, val, count) for (int i = 0; i < count; i++) arr[i] = val;

#endif //MEM_UTILS_H