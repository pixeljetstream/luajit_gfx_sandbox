local ffi = require "ffi"

ffi.cdef [[
// lxp | Lux Platform
typedef unsigned char uchar ;
typedef unsigned short ushort ;
typedef unsigned int uint ;
typedef unsigned long ulong ;
typedef unsigned char byte ;
typedef unsigned int booln ;
typedef char int8 ;
typedef short int16 ;
typedef int int32 ;
typedef long long int64 ;
typedef unsigned char uint8 ;
typedef unsigned short uint16 ;
typedef unsigned int uint32 ;
typedef unsigned long long uint64 ;
typedef unsigned char bool8 ;
typedef unsigned short bool16 ;
typedef unsigned int bool32 ;
typedef unsigned int enum32 ;
typedef unsigned int flags8 ;
typedef unsigned int flags16 ;
typedef unsigned int flags32 ;
typedef unsigned long long flags64 ;
typedef unsigned short float16 ;
typedef float float32 ;
typedef double float64 ;
typedef struct lxRectanglei_s * lxRectangleiPTR ;
typedef struct lxRectanglef_s * lxRectanglefPTR ;
typedef struct lxVec2i_s * lxVec2iPTR ;
typedef struct lxVec3i_s * lxVec3iPTR ;
typedef struct lxVec2f_s * lxVec2fPTR ;
typedef struct lxVec3f_s * lxVec3fPTR ;
typedef struct lxVec4f_s * lxVec4fPTR ;
typedef const struct lxRectanglei_s * lxRectangleiCPTR ;
typedef const struct lxRectanglef_s * lxRectanglefCPTR ;
typedef const struct lxVec2i_s * lxVec2iCPTR ;
typedef const struct lxVec3i_s * lxVec3iCPTR ;
typedef const struct lxVec2f_s * lxVec2fCPTR ;
typedef const struct lxVec3f_s * lxVec3fCPTR ;
typedef const struct lxVec4f_s * lxVec4fCPTR ;
typedef struct lxRectanglei_s
{
    int x ;
    int y ;
    int width ;
    int height ;
}
lxRectanglei_t ;
typedef struct lxRectanglef_s
{
    float x ;
    float y ;
    float width ;
    float height ;
}
lxRectanglef_t ;
typedef struct lxVec2i_s
{
    int x ;
    int y ;
}
lxVec2i_t ;
typedef struct lxVec3i_s
{
    int x ;
    int y ;
    int z ;
}
lxVec3i_t ;
typedef struct lxVec2f_s
{
    float x ;
    float y ;
}
lxVec2f_t ;
typedef struct lxVec3f_s
{
    float x ;
    float y ;
    float z ;
}
lxVec3f_t ;
typedef struct lxVec4f_s
{
    float x ;
    float y ;
    float z ;
    float w ;
}
lxVec4f_t ;
typedef enum lxScalarType_e
{
    LUX_SCALAR_FLOAT32 , LUX_SCALAR_INT8 , LUX_SCALAR_UINT8 , LUX_SCALAR_INT16 , LUX_SCALAR_UINT16 , LUX_SCALAR_INT32 , LUX_SCALAR_UINT32 , LUX_SCALAR_FLOAT16 , LUX_SCALAR_FLOAT64 , LUX_SCALAR_ILLEGAL , LUX_SCALARS , }
lxScalarType_t ;
typedef enum lxBoolean_e
{
    LUX_FALSE = 0 , LUX_TRUE = 1 , }
lxBoolean_t ;
]]

return ffi.load("luxbackend")
