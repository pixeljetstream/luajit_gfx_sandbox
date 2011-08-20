// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxmath/misc.h>


LUX_API float lxGetAverageOfN (const float *array, int n)
{
  float avg;
  int i;

  i = n;
  avg = 0;
  while (i--) avg+=array[i];

  return avg/n;
}

LUX_API float lxGetMinOfN (const float *array, int n) {
  float min;

  min = *array;
  while (n--) if (min>*array) min = *(array++); else array++;

  return min;
}

LUX_API float lxGetMaxOfN (const float *array, int n) {
  float max;

  max = *array;
  while (n--) if (max<*array) max = *(array++); else array++;

  return max;
}

LUX_API float lxGetIntAverageOfN (const int *array, int n)
{
  float avg;
  int i;

  i = n;
  avg = 0;
  while (i--) avg+=array[i];

  return avg/n;
}

LUX_API int lxGetIntMinOfN (const int *array, int n) {
  int min;

  min = *array;
  while (n--) if (min>*array) min = *(array++); else array++;

  return min;
}

LUX_API int lxGetIntMaxOfN (const int *array, int n) {
  int max;

  max = *array;
  while (n--) if (max<*array) max = *(array++); else array++;

  return max;
}

LUX_API void  lxPermutation(uint32 *out, int32 n)
{
  // Fisher–Yates shuffle
  int32 i;
  for (i = 0; i < n; i++){
    out[i] = i;
  }
  for (i = n-1; i > 0; i-- ){
    int32 k = (rand()<<15 | rand()) % (i+1);
    uint32 temp = out[i];
    out[i] = out[k];
    out[k] = temp;
  }
}
