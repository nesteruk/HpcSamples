#include "stdafx.h"

void add(float *a, float *b, float *c, float *d, float *e, int count)
{
  int i;
  #pragma simd
  for (i = 0; i < count; i++)
  {
    a[i] = a[i] + b[i] + c[i] * d[i] + e[i];
  }
}