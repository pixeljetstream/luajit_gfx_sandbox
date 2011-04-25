local ffi = require "ffi"

ffi.cdef [[
static const unsigned int LUX_FALSE = 0;
static const unsigned int LUX_TRUE = 1;

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned long   ulong;

typedef unsigned char byte;
typedef unsigned int  booln;

typedef char        int8;
typedef short       int16;
typedef int         int32;
typedef long long   int64;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

typedef unsigned char   bool8;
typedef unsigned short  bool16;
typedef unsigned int    bool32;

typedef unsigned int        enum32;
typedef unsigned int        flags32;
typedef unsigned long long  flags64;

typedef unsigned short  float16;
typedef float           float32;
typedef double          float64;

typedef struct lxRectanglei_s{
  int   x;
  int   y;
  int   width;
  int   height;
}lxRectanglei_t;
typedef lxRectanglei_t* lxRectangleiPTR;

typedef struct lxRectanglef_s{
  float x;
  float y;
  float width;
  float height;
}lxRectanglef_t;
typedef struct lxRectanglef_s* lxRectanglefPTR;

typedef struct lxVec2i_s{
  int   x;
  int   y;
}lxVec2i_t;
typedef struct lxVec2i_s* lxVec2iPTR;

typedef struct lxVec3i_s{
  int   x;
  int   y;
  int   z;
}lxVec3i_t;
typedef struct lxVec3i_s* lxVec3iPTR;

typedef struct lxVec2f_s{
  float   x;
  float   y;
}lxVec2f_t;
typedef struct lxVec2f_s* lxVec2fPTR;

typedef struct lxVec3f_s{
  float   x;
  float   y;
  float   z;
}lxVec3f_t;
typedef struct lxVec3f_s* lxVec3fPTR;

typedef struct lxVec4f_s{
  float   x;
  float   y;
  float   z;
  float   w;
}lxVec4f_t;
typedef struct lxVec4f_s* lxVec4fPTR;

typedef enum lxScalarType_e{  
  // warning may not change order!!
  // interpolation is based on abs(max)
  LUX_SCALAR_FLOAT32,   // sat: [0,1]
  LUX_SCALAR_INT8,    // sat: [-127,127]
  LUX_SCALAR_UINT8,   // sat: [0,255]               
  LUX_SCALAR_INT16,   // sat: [-SHORT_SIGNEDMAX,SHORT_SIGNEDMAX]
  LUX_SCALAR_UINT16,    // sat: [0,SHORT_UNSIGNEDMAX]
  LUX_SCALAR_INT32,   // sat: same as INT16
  LUX_SCALAR_UINT32,    // sat: same as UINT16

  LUX_SCALAR_FLOAT16,   // not supported in conversion and array ops
  LUX_SCALAR_FLOAT64,
  LUX_SCALAR_ILLEGAL,
  LUX_SCALARS,
}lxScalarType_t;
]]

return {}
