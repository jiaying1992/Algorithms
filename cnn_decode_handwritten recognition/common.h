#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef char           S8;
typedef unsigned char  U8;
typedef short          S16;
typedef unsigned short U16;
typedef int            S32;
typedef unsigned int   U32;
typedef float          F32;

#ifndef MAX
#define MAX(a,b)       ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b)       ((a)<(b)?(a):(b))
#endif

#ifndef MIN_S16
#define MIN_S16         ((short)0x8000)
#endif

#ifndef MAX_S16
#define MAX_S16         ((short)0x7FFF)
#endif

#endif
