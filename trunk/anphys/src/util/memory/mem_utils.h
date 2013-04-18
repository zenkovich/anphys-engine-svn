#ifndef MEM_UTILS_H
#define MEM_UTILS_H

#define safe_release(obj) if (obj != 0) { delete obj; obj = 0; }
#define safe_release_arr(arr) if (arr != 0) { delete[] arr; arr = 0; }

#define copyCycle(dest, desc, count) for (int i = 0; i < count; i++) dest[i] = desc[i];
#define defineArray(arr, val, count) for (int i = 0; i < count; i++) arr[i] = val;

#endif //MEM_UTILS_H