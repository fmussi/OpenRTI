#ifndef _SHLIB_H__
#define _SHLIB_H__

#include "shlib_EXPORTS.h"

#ifdef __cplusplus
extern "C" {
#endif

void SHARED_EXPORT f();

class SHARED_EXPORT X {
public:
  X();
  void mX();
};

#endif

#ifdef __cplusplus
} // extern "C"
#endif