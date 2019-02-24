// ----------------------------------------------------------------------------
// 
// 
// ----------------------------------------------------------------------------
#ifndef MISC_H
#define MISC_H

// ----------------------------------------------------------------------------
#include <stdint.h>

// ...
void misc_clamp(float* val, float min, float max)
{
  if((*val) > max)
  {
    (*val) = max;
  }
  else if((*val) < min)
  {
    (*val) = min;
  }
}

#endif