local ffi = require "ffi"
require "luxinia2.luxplatform"
require "glewgl"

ffi.cdef [[
// lxg | Lux Graphics
typedef struct lxgContext_s * lxgContextPTR ;
typedef struct lxgBuffer_s * lxgBufferPTR ;
typedef struct lxgStreamHost_s * lxgStreamHostPTR ;
typedef struct lxgVertexDecl_s * lxgVertexDeclPTR ;
typedef struct lxgFeedbackState_s * lxgFeedbackStatePTR ;
typedef struct lxgTextureImage_s * lxgTextureImagePTR ;
typedef struct lxgSampler_s * lxgSamplerPTR ;
typedef struct lxgTexture_s * lxgTexturePTR ;
typedef struct lxgRenderBuffer_s * lxgRenderBufferPTR ;
typedef struct lxgTextureUpdate_s * lxgTextureUpdatePTR ;
typedef struct lxgProgramParameter_s * lxgProgramParameterPTR ;
typedef struct lxgStageProgram_s * lxgStageProgramPTR ;
typedef struct lxgProgram_s * lxgProgramPTR ;
typedef struct lxgRenderTarget_s * lxgRenderTargetPTR ;
typedef struct lxgViewPort_s * lxgViewPortPTR ;
typedef struct lxgViewPortMrt_s * lxgViewPortMrtPTR ;
typedef struct lxgFrameBounds_s * lxgFrameBoundsPTR ;
typedef struct lxgRenderAssign_s * lxgRenderAssignPTR ;
typedef struct lxgBlend_s * lxgBlendPTR ;
typedef struct lxgStencil_s * lxgStencilPTR ;
typedef struct lxgLogic_s * lxgLogicPTR ;
typedef struct lxgDepth_s * lxgDepthPTR ;
typedef struct lxgColor_s * lxgColorPTR ;
typedef struct lxgRasterizer_s * lxgRasterizerPTR ;
typedef const struct lxgContext_s * lxgContextCPTR ;
typedef const struct lxgBuffer_s * lxgBufferCPTR ;
typedef const struct lxgStreamHost_s * lxgStreamHostCPTR ;
typedef const struct lxgVertexDecl_s * lxgVertexDeclCPTR ;
typedef const struct lxgFeedbackState_s * lxgFeedbackStateCPTR ;
typedef const struct lxgTextureImage_s * lxgTextureImageCPTR ;
typedef const struct lxgSampler_s * lxgSamplerCPTR ;
typedef const struct lxgTexture_s * lxgTextureCPTR ;
typedef const struct lxgRenderBuffer_s * lxgRenderBufferCPTR ;
typedef const struct lxgTextureUpdate_s * lxgTextureUpdateCPTR ;
typedef const struct lxgProgramParameter_s * lxgProgramParameterCPTR ;
typedef const struct lxgStageProgram_s * lxgStageProgramCPTR ;
typedef const struct lxgProgram_s * lxgProgramCPTR ;
typedef const struct lxgRenderTarget_s * lxgRenderTargetCPTR ;
typedef const struct lxgViewPort_s * lxgViewPortCPTR ;
typedef const struct lxgViewPortMrt_s * lxgViewPortMrtCPTR ;
typedef const struct lxgFrameBounds_s * lxgFrameBoundsCPTR ;
typedef const struct lxgRenderAssign_s * lxgRenderAssignCPTR ;
typedef const struct lxgBlend_s * lxgBlendCPTR ;
typedef const struct lxgColor_s * lxgColorCPTR ;
typedef const struct lxgStencil_s * lxgStencilCPTR ;
typedef const struct lxgLogic_s * lxgLogicCPTR ;
typedef const struct lxgDepth_s * lxgDepthCPTR ;
typedef const struct lxgRasterizer_s * lxgRasterizerCPTR ;
typedef flags32 lxgRenderFlag_t ;
typedef enum lxgAccessMode_e
{
    LUXGFX_ACCESS_READ , LUXGFX_ACCESS_WRITE , LUXGFX_ACCESS_READWRITE , LUXGFX_ACCESS_WRITEDISCARD , LUXGFX_ACCESS_WRITEDISCARDALL , LUXGFX_ACCESSES , }
lxgAccessMode_t ;
enum
{
    LUXGFX_MAX_TEXTURE_IMAGES = 32 , LUXGFX_MAX_RENDERTARGETS = 16 , LUXGFX_MAX_RWTEXTURE_IMAGES = 8 , LUXGFX_MAX_STAGE_BUFFERS = 12 , LUXGFX_MAX_TEXTURE_MIPMAPS = 16 , LUXGFX_MAX_VERTEX_STREAMS = 8 , LUXGFX_MAX_STAGE_SUBROUTINES = 1024 , }
;
typedef enum lxGLCompareMode_e
{
    LUXGL_COMPARE_NEVER = GL_NEVER , LUXGL_COMPARE_LESS = GL_LESS , LUXGL_COMPARE_EQUAL = GL_EQUAL , LUXGL_COMPARE_LEQUAL = GL_LEQUAL , LUXGL_COMPARE_GREATER = GL_GREATER , LUXGL_COMPARE_NOTEQUAL = GL_NOTEQUAL , LUXGL_COMPARE_GEQUAL = GL_GEQUAL , LUXGL_COMPARE_ALWAYS = GL_ALWAYS , LUXGL_COMPARE_DONTEXECUTE = 0xFFFFFFFFu , }
lxGLCompareMode_t ;
typedef enum lxGLBufferHint_e
{
    LUXGL_STATIC_DRAW = GL_STATIC_DRAW , LUXGL_STATIC_READ = GL_STATIC_READ , LUXGL_STATIC_COPY = GL_STATIC_COPY , LUXGL_DYNAMIC_DRAW = GL_DYNAMIC_DRAW , LUXGL_DYNAMIC_READ = GL_DYNAMIC_READ , LUXGL_DYNAMIC_COPY = GL_DYNAMIC_COPY , LUXGL_STREAM_DRAW = GL_STREAM_DRAW , LUXGL_STREAM_READ = GL_STREAM_READ , LUXGL_STREAM_COPY = GL_STREAM_COPY , }
lxGLBufferHint_t ;
typedef enum lxGLStencilMode_e
{
    LUXGL_STENCIL_KEEP = GL_KEEP , LUXGL_STENCIL_ZERO = GL_ZERO , LUXGL_STENCIL_REPLACE = GL_REPLACE , LUXGL_STENCIL_INCR_SAT = GL_INCR , LUXGL_STENCIL_DECR_SAT = GL_DECR , LUXGL_STENCIL_INVERT = GL_INVERT , LUXGL_STENCIL_INCR = GL_INCR_WRAP , LUXGL_STENCIL_DECR = GL_DECR_WRAP , }
lxGLStencilMode_t ;
typedef enum lxGLBlendWeight_e
{
    LUXGL_BLENDW_ZERO = GL_ZERO , LUXGL_BLENDW_ONE = GL_ONE , LUXGL_BLENDW_RGB_SRC = GL_SRC_COLOR , LUXGL_BLENDW_RGB_DST = GL_DST_COLOR , LUXGL_BLENDW_A_SRC = GL_SRC_ALPHA , LUXGL_BLENDW_A_DST = GL_DST_ALPHA , LUXGL_BLENDW_INVRGB_SRC = GL_ONE_MINUS_SRC_COLOR , LUXGL_BLENDW_INVRGB_DST = GL_ONE_MINUS_DST_COLOR , LUXGL_BLENDW_INVA_SRC = GL_ONE_MINUS_SRC_ALPHA , LUXGL_BLENDW_INVA_DST = GL_ONE_MINUS_DST_ALPHA , LUXGL_BLENDW_UNKOWN = 0xFFFFABCD , }
lxGLBlendWeight_t ;
typedef enum lxGLBlendEquation_e
{
    LUXGL_BLENDE_ADD = GL_FUNC_ADD , LUXGL_BLENDE_SUB = GL_FUNC_SUBTRACT , LUXGL_BLENDE_SUB_REV = GL_FUNC_REVERSE_SUBTRACT , LUXGL_BLENDE_MIN = GL_MIN , LUXGL_BLENDE_MAX = GL_MAX , LUXGL_BLENDE_UNKOWN = 0xFFFFABCD , }
lxGLBlendEquation_t ;
typedef enum lxGLLogicOp_e
{
    LUXGL_LOGICOP_CLEAR = GL_CLEAR , LUXGL_LOGICOP_SET = GL_SET , LUXGL_LOGICOP_COPY = GL_COPY , LUXGL_LOGICOP_INVERTED = GL_COPY_INVERTED , LUXGL_LOGICOP_NOOP = GL_NOOP , LUXGL_LOGICOP_INVERT = GL_INVERT , LUXGL_LOGICOP_AND = GL_AND , LUXGL_LOGICOP_NAND = GL_NAND , LUXGL_LOGICOP_OR = GL_OR , LUXGL_LOGICOP_NOR = GL_NOR , LUXGL_LOGICOP_XOR = GL_XOR , LUXGL_LOGICOP_EQUIV = GL_EQUIV , LUXGL_LOGICOP_AND_REVERSE = GL_AND_REVERSE , LUXGL_LOGICOP_AND_INVERTED = GL_AND_INVERTED , LUXGL_LOGICOP_OR_REVERSE = GL_OR_REVERSE , LUXGL_LOGICOP_OR_INVERTED = GL_OR_INVERTED , LUXGL_LOGICOP_ILLEGAL = 0 , }
lxGLLogicOp_t ;
typedef enum lxGLPrimitiveType_e
{
    LUXGL_POINTS = GL_POINTS , LUXGL_TRIANGLES = GL_TRIANGLES , LUXGL_TRIANGLE_STRIP = GL_TRIANGLE_STRIP , LUXGL_TRIANGLE_FAN = GL_TRIANGLE_FAN , LUXGL_LINES = GL_LINES , LUXGL_LINE_LOOP = GL_LINE_LOOP , LUXGL_LINE_STRIP = GL_LINE_STRIP , LUXGL_QUADS = GL_QUADS , LUXGL_QUAD_STRIP = GL_QUAD_STRIP , LUXGL_LINE_ADJ = GL_LINES_ADJACENCY , LUXGL_LINE_STRIP_ADJ = GL_LINE_STRIP_ADJACENCY , LUXGL_TRIANGLE_STRIP_ADJ = GL_TRIANGLE_STRIP_ADJACENCY , LUXGL_TRIANGLE_ADJ = GL_TRIANGLES_ADJACENCY , LUXGL_PATCHES = GL_PATCHES , LUXGL_POLYGON = GL_POLYGON , }
lxGLPrimitiveType_t ;
typedef enum lxGLTextureTarget_e
{
    LUXGL_TEXTURE_1D = GL_TEXTURE_1D , LUXGL_TEXTURE_2D = GL_TEXTURE_2D , LUXGL_TEXTURE_3D = GL_TEXTURE_3D , LUXGL_TEXTURE_RECT = GL_TEXTURE_RECTANGLE , LUXGL_TEXTURE_1DARRAY = GL_TEXTURE_1D_ARRAY , LUXGL_TEXTURE_2DARRAY = GL_TEXTURE_2D_ARRAY , LUXGL_TEXTURE_CUBE = GL_TEXTURE_CUBE_MAP , LUXGL_TEXTURE_CUBEARRAY = GL_TEXTURE_CUBE_MAP_ARRAY , LUXGL_TEXTURE_2DMS = GL_TEXTURE_2D_MULTISAMPLE , LUXGL_TEXTURE_2DMSARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY , LUXGL_TEXTURE_BUFFER = GL_TEXTURE_BUFFER , LUXGL_TEXTURE_RENDERBUFFER = GL_TEXTURE_RENDERBUFFER_NV , LUXGL_TEXTURE_INVALID = 0 , }
lxGLTextureTarget_t ;
typedef enum lxGLBufferTarget_e
{
    LUXGL_BUFFER_VERTEX = GL_ARRAY_BUFFER , LUXGL_BUFFER_INDEX = GL_ELEMENT_ARRAY_BUFFER , LUXGL_BUFFER_PIXELWRITE = GL_PIXEL_PACK_BUFFER , LUXGL_BUFFER_PIXELREAD = GL_PIXEL_UNPACK_BUFFER , LUXGL_BUFFER_UNIFORM = GL_UNIFORM_BUFFER , LUXGL_BUFFER_TEXTURE = GL_TEXTURE_BUFFER , LUXGL_BUFFER_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER , LUXGL_BUFFER_CPYWRITE = GL_COPY_WRITE_BUFFER , LUXGL_BUFFER_CPYREAD = GL_COPY_READ_BUFFER , LUXGL_BUFFER_DRAWINDIRECT = GL_DRAW_INDIRECT_BUFFER , LUXGL_BUFFER_NVVIDEO = 0x9020 , LUXGL_BUFFER_NVPARAM_VERTEX = GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV , LUXGL_BUFFER_NVPARAM_GEOMETRY = GL_GEOMETRY_PROGRAM_PARAMETER_BUFFER_NV , LUXGL_BUFFER_NVPARAM_FRAGMENT = GL_FRAGMENT_PROGRAM_PARAMETER_BUFFER_NV , LUXGL_BUFFER_NVPARAM_TESSCTRL = GL_TESS_CONTROL_PROGRAM_PARAMETER_BUFFER_NV , LUXGL_BUFFER_NVPARAM_TESSEVAL = GL_TESS_EVALUATION_PROGRAM_PARAMETER_BUFFER_NV , LUXGL_BUFFER_INVALID = 0 , }
lxGLBufferTarget_t ;
typedef enum lxGLShaderType_e
{
    LUXGL_SHADER_VERTEX = GL_VERTEX_SHADER , LUXGL_SHADER_FRAGMENT = GL_FRAGMENT_SHADER , LUXGL_SHADER_GEOMETRY = GL_GEOMETRY_SHADER , LUXGL_SHADER_TESSCTRL = GL_TESS_CONTROL_SHADER , LUXGL_SHADER_TESSEVAL = GL_TESS_EVALUATION_SHADER , }
lxGLShaderType_t ;
typedef enum lxGLProgramType_e
{
    LUXGL_PROGRAM_VERTEX = GL_VERTEX_PROGRAM_ARB , LUXGL_PROGRAM_FRAGMENT = GL_FRAGMENT_PROGRAM_ARB , LUXGL_PROGRAM_GEOMETRY = GL_GEOMETRY_PROGRAM_NV , LUXGL_PROGRAM_TESSCTRL = GL_TESS_CONTROL_PROGRAM_NV , LUXGL_PROGRAM_TESSEVAL = GL_TESS_EVALUATION_PROGRAM_NV , }
lxGLProgramType_t ;
typedef enum lxGLAccessFormat_e
{
    LUXGL_ACCESSFORMAT_R8UI = GL_R8UI , LUXGL_ACCESSFORMAT_R8I = GL_R8I , LUXGL_ACCESSFORMAT_R16UI = GL_R16UI , LUXGL_ACCESSFORMAT_R16I = GL_R16I , LUXGL_ACCESSFORMAT_R32UI = GL_R32UI , LUXGL_ACCESSFORMAT_R32I = GL_R32I , LUXGL_ACCESSFORMAT_R32F = GL_R32F , LUXGL_ACCESSFORMAT_RG32UI = GL_RG32UI , LUXGL_ACCESSFORMAT_RG32I = GL_RG32I , LUXGL_ACCESSFORMAT_RG32F = GL_RG32F , LUXGL_ACCESSFORMAT_RGBA32UI = GL_RGBA32UI , LUXGL_ACCESSFORMAT_RGBA32I = GL_RGBA32I , LUXGL_ACCESSFORMAT_RGBA32F = GL_RGBA32F , }
lxGLAccessFormat_t ;
typedef enum lxGLAccessMode_e
{
    LUXGL_ACCESS_READ_ONLY = GL_READ_ONLY , LUXGL_ACCESS_WRITE_ONLY = GL_WRITE_ONLY , LUXGL_ACCESS_READ_WRITE = GL_READ_WRITE , }
lxGLAccessMode_t ;
typedef enum lxGLParameterType_e
{
    LUXGL_PARAM_FLOAT = GL_FLOAT , LUXGL_PARAM_FLOAT2 = GL_FLOAT_VEC2 , LUXGL_PARAM_FLOAT3 = GL_FLOAT_VEC3 , LUXGL_PARAM_FLOAT4 = GL_FLOAT_VEC4 , LUXGL_PARAM_INT = GL_INT , LUXGL_PARAM_INT2 = GL_INT_VEC2 , LUXGL_PARAM_INT3 = GL_INT_VEC3 , LUXGL_PARAM_INT4 = GL_INT_VEC4 , LUXGL_PARAM_UINT = GL_UNSIGNED_INT , LUXGL_PARAM_UINT2 = GL_UNSIGNED_INT_VEC2 , LUXGL_PARAM_UINT3 = GL_UNSIGNED_INT_VEC3 , LUXGL_PARAM_UINT4 = GL_UNSIGNED_INT_VEC4 , LUXGL_PARAM_BOOL = GL_BOOL , LUXGL_PARAM_BOOL2 = GL_BOOL_VEC2 , LUXGL_PARAM_BOOL3 = GL_BOOL_VEC3 , LUXGL_PARAM_BOOL4 = GL_BOOL_VEC4 , LUXGL_PARAM_MAT2 = GL_FLOAT_MAT2 , LUXGL_PARAM_MAT3 = GL_FLOAT_MAT3 , LUXGL_PARAM_MAT4 = GL_FLOAT_MAT4 , LUXGL_PARAM_MAT2x3 = GL_FLOAT_MAT2x3 , LUXGL_PARAM_MAT2x4 = GL_FLOAT_MAT2x4 , LUXGL_PARAM_MAT3x2 = GL_FLOAT_MAT3x2 , LUXGL_PARAM_MAT3x4 = GL_FLOAT_MAT3x4 , LUXGL_PARAM_MAT4x2 = GL_FLOAT_MAT4x2 , LUXGL_PARAM_MAT4x3 = GL_FLOAT_MAT4x3 , LUXGL_PARAM_SAMPLER_1D = GL_SAMPLER_1D , LUXGL_PARAM_SAMPLER_2D = GL_SAMPLER_2D , LUXGL_PARAM_SAMPLER_3D = GL_SAMPLER_3D , LUXGL_PARAM_SAMPLER_CUBE = GL_SAMPLER_CUBE , LUXGL_PARAM_SAMPLER_2DRECT = GL_SAMPLER_2D_RECT , LUXGL_PARAM_SAMPLER_2DMS = GL_SAMPLER_2D_MULTISAMPLE , LUXGL_PARAM_SAMPLER_1DARRAY = GL_SAMPLER_1D_ARRAY , LUXGL_PARAM_SAMPLER_2DARRAY = GL_SAMPLER_2D_ARRAY , LUXGL_PARAM_SAMPLER_CUBEARRAY = GL_SAMPLER_CUBE_MAP_ARRAY , LUXGL_PARAM_SAMPLER_2DMSARRAY = GL_SAMPLER_2D_MULTISAMPLE_ARRAY , LUXGL_PARAM_SAMPLER_BUFFER = GL_SAMPLER_BUFFER , LUXGL_PARAM_ISAMPLER_1D = GL_INT_SAMPLER_1D , LUXGL_PARAM_ISAMPLER_2D = GL_INT_SAMPLER_2D , LUXGL_PARAM_ISAMPLER_3D = GL_INT_SAMPLER_3D , LUXGL_PARAM_ISAMPLER_CUBE = GL_INT_SAMPLER_CUBE , LUXGL_PARAM_ISAMPLER_2DRECT = GL_INT_SAMPLER_2D_RECT , LUXGL_PARAM_ISAMPLER_2DMS = GL_INT_SAMPLER_2D_MULTISAMPLE , LUXGL_PARAM_ISAMPLER_1DARRAY = GL_INT_SAMPLER_1D_ARRAY , LUXGL_PARAM_ISAMPLER_2DARRAY = GL_INT_SAMPLER_2D_ARRAY , LUXGL_PARAM_ISAMPLER_CUBEARRAY = GL_INT_SAMPLER_CUBE_MAP_ARRAY , LUXGL_PARAM_ISAMPLER_2DMSARRAY = GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY , LUXGL_PARAM_ISAMPLER_BUFFER = GL_INT_SAMPLER_BUFFER , LUXGL_PARAM_USAMPLER_1D = GL_UNSIGNED_INT_SAMPLER_1D , LUXGL_PARAM_USAMPLER_2D = GL_UNSIGNED_INT_SAMPLER_2D , LUXGL_PARAM_USAMPLER_3D = GL_UNSIGNED_INT_SAMPLER_3D , LUXGL_PARAM_USAMPLER_CUBE = GL_UNSIGNED_INT_SAMPLER_CUBE , LUXGL_PARAM_USAMPLER_2DRECT = GL_UNSIGNED_INT_SAMPLER_2D_RECT , LUXGL_PARAM_USAMPLER_2DMS = GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE , LUXGL_PARAM_USAMPLER_1DARRAY = GL_UNSIGNED_INT_SAMPLER_1D_ARRAY , LUXGL_PARAM_USAMPLER_2DARRAY = GL_UNSIGNED_INT_SAMPLER_2D_ARRAY , LUXGL_PARAM_USAMPLER_CUBEARRAY = GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY , LUXGL_PARAM_USAMPLER_2DMSARRAY = GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY , LUXGL_PARAM_USAMPLER_BUFFER = GL_UNSIGNED_INT_SAMPLER_BUFFER , LUXGL_PARAM_SAMPLER_1D_SHADOW = GL_SAMPLER_1D_SHADOW , LUXGL_PARAM_SAMPLER_2D_SHADOW = GL_SAMPLER_2D_SHADOW , LUXGL_PARAM_SAMPLER_CUBE_SHADOW = GL_SAMPLER_CUBE_SHADOW , LUXGL_PARAM_SAMPLER_2DRECT_SHADOW = GL_SAMPLER_2D_RECT_SHADOW , LUXGL_PARAM_SAMPLER_1DARRAY_SHADOW = GL_SAMPLER_1D_ARRAY_SHADOW , LUXGL_PARAM_SAMPLER_2DARRAY_SHADOW = GL_SAMPLER_2D_ARRAY_SHADOW , LUXGL_PARAM_SAMPLER_CUBEARRAY_SHADOW = GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW , LUXGL_PARAM_IMAGE_1D = GL_IMAGE_1D_EXT , LUXGL_PARAM_IMAGE_2D = GL_IMAGE_2D_EXT , LUXGL_PARAM_IMAGE_3D = GL_IMAGE_3D_EXT , LUXGL_PARAM_IMAGE_CUBE = GL_IMAGE_CUBE_EXT , LUXGL_PARAM_IMAGE_2DRECT = GL_IMAGE_2D_RECT_EXT , LUXGL_PARAM_IMAGE_2DMS = GL_IMAGE_2D_MULTISAMPLE_EXT , LUXGL_PARAM_IMAGE_1DARRAY = GL_IMAGE_1D_ARRAY_EXT , LUXGL_PARAM_IMAGE_2DARRAY = GL_IMAGE_2D_ARRAY_EXT , LUXGL_PARAM_IMAGE_CUBEARRAY = GL_IMAGE_CUBE_MAP_ARRAY_EXT , LUXGL_PARAM_IMAGE_2DMSARRAY = GL_IMAGE_2D_MULTISAMPLE_ARRAY_EXT , LUXGL_PARAM_IMAGE_BUFFER = GL_IMAGE_BUFFER_EXT , LUXGL_PARAM_IIMAGE_1D = GL_INT_IMAGE_1D_EXT , LUXGL_PARAM_IIMAGE_2D = GL_INT_IMAGE_2D_EXT , LUXGL_PARAM_IIMAGE_3D = GL_INT_IMAGE_3D_EXT , LUXGL_PARAM_IIMAGE_CUBE = GL_INT_IMAGE_CUBE_EXT , LUXGL_PARAM_IIMAGE_2DRECT = GL_INT_IMAGE_2D_RECT_EXT , LUXGL_PARAM_IIMAGE_2DMS = GL_INT_IMAGE_2D_MULTISAMPLE_EXT , LUXGL_PARAM_IIMAGE_1DARRAY = GL_INT_IMAGE_1D_ARRAY_EXT , LUXGL_PARAM_IIMAGE_2DARRAY = GL_INT_IMAGE_2D_ARRAY_EXT , LUXGL_PARAM_IIMAGE_CUBEARRAY = GL_INT_IMAGE_CUBE_MAP_ARRAY_EXT , LUXGL_PARAM_IIMAGE_2DMSARRAY = GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT , LUXGL_PARAM_IIMAGE_BUFFER = GL_INT_IMAGE_BUFFER_EXT , LUXGL_PARAM_UIMAGE_1D = GL_UNSIGNED_INT_IMAGE_1D_EXT , LUXGL_PARAM_UIMAGE_2D = GL_UNSIGNED_INT_IMAGE_2D_EXT , LUXGL_PARAM_UIMAGE_3D = GL_UNSIGNED_INT_IMAGE_3D_EXT , LUXGL_PARAM_UIMAGE_CUBE = GL_UNSIGNED_INT_IMAGE_CUBE_EXT , LUXGL_PARAM_UIMAGE_2DRECT = GL_UNSIGNED_INT_IMAGE_2D_RECT_EXT , LUXGL_PARAM_UIMAGE_2DMS = GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_EXT , LUXGL_PARAM_UIMAGE_1DARRAY = GL_UNSIGNED_INT_IMAGE_1D_ARRAY_EXT , LUXGL_PARAM_UIMAGE_2DARRAY = GL_UNSIGNED_INT_IMAGE_2D_ARRAY_EXT , LUXGL_PARAM_UIMAGE_CUBEARRAY = GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY_EXT , LUXGL_PARAM_UIMAGE_2DMSARRAY = GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT , LUXGL_PARAM_UIMAGE_BUFFER = GL_UNSIGNED_INT_IMAGE_BUFFER_EXT , LUXGL_PARAM_GPU_ADDRESS = GL_GPU_ADDRESS_NV , LUXGL_PARAM_BUFFER = 0x7FFFFFF0 , LUXGL_PARAM_SUBROUTINE = 0x7FFFFFF1 , LUXGL_PARAM_USER = 0x7FFFFFFF , }
lxGLParameterType_t ;
typedef enum lxGLError_e
{
    LUXGL_ERROR_NONE = GL_NO_ERROR , LUXGL_ERROR_OP = GL_INVALID_OPERATION , LUXGL_ERROR_ENUM = GL_INVALID_ENUM , LUXGL_ERROR_VALUE = GL_INVALID_VALUE , LUXGL_ERROR_INDEX = GL_INVALID_INDEX , LUXGL_ERROR_FBOP = GL_INVALID_FRAMEBUFFER_OPERATION , }
lxGLError_t ;
typedef struct lxgBuffer_s
{
    lxGLBufferTarget_t gltarget ;
    GLuint glid ;
    GLuint64 address ;
    flags32 ctxcapbits ;
    void * user ;
    void * mapped ;
    lxgAccessMode_t maptype ;
    uint mapstart ;
    uint maplength ;
    uint size ;
    uint used ;
    lxGLBufferHint_t hint ;
    lxgContextPTR ctx ;
}
lxgBuffer_t ;
uint lxgBuffer_alloc ( lxgBufferPTR buffer , uint needed , uint padsize ) ;
void lxgBuffer_bind ( lxgBufferCPTR buffer , lxGLBufferTarget_t type ) ;
void lxgBuffer_bindIndexed ( lxgBufferCPTR buffer , lxGLBufferTarget_t type , uint idx ) ;
void lxgBuffer_bindRanged ( lxgBufferCPTR buffer , lxGLBufferTarget_t type , uint idx , size_t offset , size_t size ) ;
void * lxgBuffer_map ( lxgBufferPTR buffer , lxgAccessMode_t type , booln * succ ) ;
void * lxgBuffer_mapRange ( lxgBufferPTR buffer , uint from , uint length , lxgAccessMode_t type , booln manualflush , booln unsynch , booln * succ ) ;
booln lxgBuffer_flushRange ( lxgBufferPTR buffer , uint from , uint length ) ;
booln lxgBuffer_unmap ( lxgBufferPTR buffer ) ;
booln lxgBuffer_copy ( lxgBufferPTR buffer , uint bufferoffset , lxgBufferPTR src , uint srcoffset , uint size ) ;
GLuint64 lxgBuffer_addressNV ( lxgBufferPTR buffer ) ;
void lxgBuffer_residentNV ( lxgBufferPTR buffer , lxgAccessMode_t mode ) ;
void lxgBuffer_unresidentNV ( lxgBufferPTR buffer ) ;
void lxgBuffer_deinit ( lxgBufferPTR buffer , lxgContextPTR ctx ) ;
void lxgBuffer_reset ( lxgBufferPTR buffer , void * data ) ;
void lxgBuffer_init ( lxgBufferPTR buffer , lxgContextPTR ctx , lxGLBufferHint_t hint , uint size , void * data ) ;
typedef enum lxgVertexAttrib_e
{
    LUXGFX_VERTEX_ATTRIB_POS , LUXGFX_VERTEX_ATTRIB_ATTR1 , LUXGFX_VERTEX_ATTRIB_NORMAL , LUXGFX_VERTEX_ATTRIB_COLOR , LUXGFX_VERTEX_ATTRIB_ATTR4 , LUXGFX_VERTEX_ATTRIB_ATTR5 , LUXGFX_VERTEX_ATTRIB_ATTR6 , LUXGFX_VERTEX_ATTRIB_ATTR7 , LUXGFX_VERTEX_ATTRIB_TEXCOORD0 , LUXGFX_VERTEX_ATTRIB_TEXCOORD1 , LUXGFX_VERTEX_ATTRIB_TEXCOORD2 , LUXGFX_VERTEX_ATTRIB_TEXCOORD3 , LUXGFX_VERTEX_ATTRIB_ATTR12 , LUXGFX_VERTEX_ATTRIB_ATTR13 , LUXGFX_VERTEX_ATTRIB_ATTR14 , LUXGFX_VERTEX_ATTRIB_ATTR15 , LUXGFX_VERTEX_ATTRIBS , }
lxgVertexAttrib_t ;
typedef struct lxgVertexElement_s
{
    unsigned normalize : 1 ;
    unsigned integer : 1 ;
    unsigned cnt : 2 ;
    unsigned stream : 4 ;
    unsigned scalartype : 8 ;
    unsigned stridehalf : 8 ;
    unsigned offset : 8 ;
}
lxgVertexElement_t ;
typedef struct lxgVertexDecl_s
{
    flags32 available ;
    uint streams ;
    lxgVertexElement_t table [ LUXGFX_VERTEX_ATTRIBS ] ;
}
lxgVertexDecl_t ;
typedef struct lxgStreamHost_s
{
    lxgBufferPTR buffer ;
    union
    {
        void * ptr ;
        size_t offset ;
    }
    ;
    size_t len ;
}
lxgStreamHost_t ;
typedef struct lxgVertexPointer_s
{
    lxgVertexElement_t element [ LUXGFX_VERTEX_ATTRIBS ] ;
    lxgStreamHost_t streams [ LUXGFX_MAX_VERTEX_STREAMS ] ;
}
lxgVertexPointer_t ;
typedef struct lxgVertexState_s
{
    lxgVertexDeclCPTR decl ;
    flags32 active ;
    flags32 declvalid ;
    flags32 declstreams ;
    flags32 streamvalid ;
    flags32 declchange ;
    flags32 streamchange ;
    lxgVertexPointer_t setup ;
}
lxgVertexState_t ;
typedef struct lxgFeedbackState_s
{
    lxGLPrimitiveType_t capture ;
    int active ;
    flags32 usedvalid ;
    flags32 streamvalid ;
    flags32 streamchange ;
    lxgStreamHost_t streams [ LUXGFX_MAX_VERTEX_STREAMS ] ;
}
lxgFeedbackState_t ;
flags32 lxgVertexAttrib_bit ( lxgVertexAttrib_t attrib ) ;
lxgVertexElement_t lxgVertexElement_set ( uint cnt , enum lxScalarType_e type , booln normalize , booln integer , uint stride , uint offset , uint stream ) ;
void lxgVertexAttrib_applyFloat ( lxgVertexAttrib_t attrib , const float * vec4 ) ;
void lxgVertexAttrib_applyInteger ( lxgVertexAttrib_t attrib , const int * vec4 ) ;
void lxgVertexAttrib_applyFloatFIXED ( lxgVertexAttrib_t attrib , const float * vec4 ) ;
void lxgContext_applyVertexAttribs ( lxgContextPTR ctx , flags32 attribs , flags32 changed ) ;
void lxgContext_applyVertexAttribsFIXED ( lxgContextPTR ctx , flags32 attribs , flags32 changed ) ;
void lxgContext_clearVertexState ( lxgContextPTR ctx ) ;
void lxgContext_setVertexDecl ( lxgContextPTR ctx , lxgVertexDeclCPTR decl ) ;
void lxgContext_setVertexDeclStreams ( lxgContextPTR ctx , lxgVertexDeclCPTR decl , lxgStreamHostCPTR hosts ) ;
void lxgContext_setVertexStream ( lxgContextPTR ctx , uint idx , lxgStreamHostCPTR host ) ;
void lxgContext_invalidateVertexStreams ( lxgContextPTR ctx ) ;
void lxgContext_applyVertexState ( lxgContextPTR ctx ) ;
void lxgContext_applyVertexStateFIXED ( lxgContextPTR ctx ) ;
void lxgContext_applyVertexStateNV ( lxgContextPTR ctx ) ;
void lxgContext_applyVertexStateFIXEDNV ( lxgContextPTR ctx ) ;
void lxgContext_clearFeedbackState ( lxgContextPTR ctx ) ;
void lxgContext_applyFeedbackStreams ( lxgContextPTR ctx ) ;
void lxgContext_setFeedbackStreams ( lxgContextPTR ctx , lxgStreamHostCPTR hosts , int numStreams ) ;
void lxgContext_setFeedbackStream ( lxgContextPTR ctx , uint idx , lxgStreamHostCPTR host ) ;
void lxgContext_enableFeedback ( lxgContextPTR ctx , lxGLPrimitiveType_t type , int numStreams ) ;
void lxgContext_disableFeedback ( lxgContextPTR ctx ) ;
typedef enum lxgSamplerFilter_e
{
    LUXGFX_SAMPLERFILTER_NEAREST , LUXGFX_SAMPLERFILTER_LINEAR , LUXGFX_SAMPLERFILTER_MIPMAP_NEAREST , LUXGFX_SAMPLERFILTER_MIPMAP_LINEAR , LUXGFX_SAMPLERFILTERS , }
lxgSamplerFilter_t ;
typedef enum lxgSamplerAddress_e
{
    LUXGFX_SAMPLERADDRESS_REPEAT , LUXGFX_SAMPLERADDRESS_MIRROR , LUXGFX_SAMPLERADDRESS_CLAMP , LUXGFX_SAMPLERADDRESS_BORDER , LUXGFX_SAMPLERADDRESSES , }
lxgSamplerAddress_t ;
enum lxgSamplerAttrib_e
{
    LUXGFX_SAMPLERATTRIB_FILTER = 1 << 0 , LUXGFX_SAMPLERATTRIB_CMP = 1 << 1 , LUXGFX_SAMPLERATTRIB_ADDRESS = 1 << 2 , LUXGFX_SAMPLERATTRIB_ANISO = 1 << 3 , LUXGFX_SAMPLERATTRIB_LOD = 1 << 4 , LUXGFX_SAMPLERATTRIB_BORDER = 1 << 5 , LUXGFX_SAMPLERATTRIB_ALL = ( 1 << 6 ) - 1 , }
;
typedef struct lxgSamplerLod_s
{
    float bias ;
    float min ;
    float max ;
}
lxgSamplerLod_t ;
typedef struct lxgSampler_s
{
    GLuint glid ;
    uint32 incarnation ;
    lxGLCompareMode_t cmpfunc ;
    lxgSamplerFilter_t filter ;
    lxgSamplerAddress_t addru ;
    lxgSamplerAddress_t addrv ;
    lxgSamplerAddress_t addrw ;
    uint aniso ;
    lxgSamplerLod_t lod ;
    float border [ 4 ] ;
}
lxgSampler_t ;
typedef enum lxgTextureFlags_e
{
    LUXGFX_TEXTUREFLAG_AUTOMIPMAP = 1 << 0 , LUXGFX_TEXTUREFLAG_MANMIPMAP = 1 << 1 , LUXGFX_TEXTUREFLAG_COMPRESS = 1 << 2 , LUXGFX_TEXTUREFLAG_COMPRESSED = 1 << 3 , LUXGFX_TEXTUREFLAG_SAMPLESFIXED = 1 << 4 , LUXGFX_TEXTUREFLAG_HASLOD = 1 << 30 , LUXGFX_TEXTUREFLAG_HASCOMPARE = 1 << 31 , }
lxgTextureFlags_t ;
typedef enum lxgTextureChannel_e
{
    LUXGFX_TEXTURECHANNEL_RGB , LUXGFX_TEXTURECHANNEL_RGBA , LUXGFX_TEXTURECHANNEL_R , LUXGFX_TEXTURECHANNEL_RG , LUXGFX_TEXTURECHANNEL_SRGB , LUXGFX_TEXTURECHANNEL_SRGBA , LUXGFX_TEXTURECHANNEL_DEPTH , LUXGFX_TEXTURECHANNEL_DEPTHSTENCIL , LUXGFX_TEXTURECHANNEL_CUSTOM , LUXGFX_TEXTURECHANNEL_NATIVE , }
lxgTextureChannel_t ;
typedef enum lxgTextureDataType_e
{
    LUXGFX_TEXTUREDATA_BASE , LUXGFX_TEXTUREDATA_UNORM8 , LUXGFX_TEXTUREDATA_UNORM16 , LUXGFX_TEXTUREDATA_SNORM8 , LUXGFX_TEXTUREDATA_SNORM16 , LUXGFX_TEXTUREDATA_FLOAT16 , LUXGFX_TEXTUREDATA_FLOAT32 , LUXGFX_TEXTUREDATA_SINT8 , LUXGFX_TEXTUREDATA_UINT8 , LUXGFX_TEXTUREDATA_SINT16 , LUXGFX_TEXTUREDATA_UINT16 , LUXGFX_TEXTUREDATA_SINT32 , LUXGFX_TEXTUREDATA_UINT32 , LUXGFX_TEXTUREDATAS , LUXGFX_TEXTUREDATA_DEPTH16 , LUXGFX_TEXTUREDATA_DEPTH24 , LUXGFX_TEXTUREDATA_DEPTH32 , LUXGFX_TEXTUREDATA_DEPTH32F , LUXGFX_TEXTUREDATA_UNORM1010102 , LUXGFX_TEXTUREDATA_UINT1010102 , LUXGFX_TEXTUREDATA_FLOAT111110 , LUXGFX_TEXTUREDATA_EXP999 , LUXGFX_TEXTUREDATA_COMPRESSED , LUXGFX_TEXTUREDATA_COMPRESSED_DXT1 , LUXGFX_TEXTUREDATA_COMPRESSED_DXT3 , LUXGFX_TEXTUREDATA_COMPRESSED_DXT5 , LUXGFX_TEXTUREDATA_COMPRESSED_TC , LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_TC , LUXGFX_TEXTUREDATA_COMPRESSED_UNORM_BPTC , LUXGFX_TEXTUREDATA_COMPRESSED_FLOAT_BPTC , LUXGFX_TEXTUREDATA_COMPRESSED_SIGNED_FLOAT_BPTC , LUXGFX_TEXTUREDATA_CUSTOM , }
lxgTextureDataType_t ;
typedef struct lxgTexture_s
{
    lxGLTextureTarget_t gltarget ;
    GLuint glid ;
    lxgSamplerCPTR lastSampler ;
    uint32 lastSamplerIncarnation ;
    lxgContextPTR ctx ;
    lxgTextureChannel_t formattype ;
    lxgTextureDataType_t datatype ;
    flags32 flags ;
    int width ;
    int height ;
    int depth ;
    int arraysize ;
    int samples ;
    flags32 mipsdefined ;
    uint miplevels ;
    lxVec3i_t mipsizes [ LUXGFX_MAX_TEXTURE_MIPMAPS ] ;
    uint pixelsizes [ LUXGFX_MAX_TEXTURE_MIPMAPS ] ;
    size_t nativesizes [ LUXGFX_MAX_TEXTURE_MIPMAPS ] ;
    uint components ;
    uint componentsize ;
    lxgSampler_t sampler ;
    GLenum glinternalformat ;
    GLenum gldatatype ;
    GLenum gldataformat ;
}
lxgTexture_t ;
typedef struct lxgRenderBuffer_s
{
    GLuint glid ;
    lxgContextPTR ctx ;
    lxgTextureChannel_t formattype ;
    int width ;
    int height ;
    uint samples ;
}
lxgRenderBuffer_t ;
typedef struct lxgTextureUpdate_s
{
    lxVec3i_t from ;
    lxVec3i_t to ;
    lxVec3i_t size ;
}
lxgTextureUpdate_t ;
typedef struct lxgTextureImage_s
{
    lxgTexturePTR tex ;
    int level ;
    booln layered ;
    int layer ;
    lxGLAccessFormat_t glformat ;
    lxGLAccessMode_t glaccess ;
}
lxgTextureImage_t ;
void lxgContext_clearTextureState ( lxgContextPTR ctx ) ;
void lxgContext_setTextureSampler ( lxgContextPTR ctx , uint imageunit , flags32 what ) ;
void lxgContext_changedTextureSampler ( lxgContextPTR ctx , uint imageunit , flags32 what ) ;
void lxgContext_applyTexture ( lxgContextPTR ctx , lxgTexturePTR obj , uint imageunit ) ;
void lxgContext_applyTextures ( lxgContextPTR ctx , lxgTexturePTR * texs , uint start , uint num ) ;
void lxgContext_applySampler ( lxgContextPTR ctx , lxgSamplerCPTR obj , uint imageunit ) ;
void lxgContext_applySamplers ( lxgContextPTR ctx , lxgSamplerCPTR * samps , uint start , uint num ) ;
void lxgContext_applyTextureImages ( lxgContextPTR ctx , lxgTextureImageCPTR * imgs , uint start , uint num ) ;
void lxgContext_applyTextureImage ( lxgContextPTR ctx , lxgTextureImageCPTR img , uint imageunit ) ;
booln lxgTextureChannel_valid ( lxgContextPTR ctx , lxgTextureChannel_t channel ) ;
booln lxgTextureTarget_valid ( lxgContextPTR ctx , lxGLTextureTarget_t type ) ;
GLenum lxgTextureDataType_getData ( lxgTextureDataType_t data , booln rev , booln depthstencil ) ;
GLenum lxgTextureChannel_getFormat ( lxgTextureChannel_t type , booln rev , booln integer ) ;
GLenum lxgTextureChannel_getInternal ( lxgTextureChannel_t type , lxgTextureDataType_t data ) ;
void lxgTexture_init ( lxgTexturePTR tex , lxgContextPTR ctx ) ;
void lxgTexture_deinit ( lxgTexturePTR tex , lxgContextPTR ctx ) ;
booln lxgTexture_setup ( lxgTexturePTR tex , lxGLTextureTarget_t type , lxgTextureChannel_t format , lxgTextureDataType_t data , int width , int height , int depth , int arraysize , flags32 flags ) ;
booln lxgTexture_resize ( lxgTexturePTR tex , int width , int height , int depth , int arraysize ) ;
booln lxgTexture_readFrame ( lxgTexturePTR tex , lxgContextPTR ctx , const lxgTextureUpdate_t * update , uint miplevel ) ;
booln lxgTexture_readData ( lxgTexturePTR tex , const lxgTextureUpdate_t * update , uint miplevel , GLenum datatype , GLenum dataformat , const void * buffer , uint buffersize ) ;
booln lxgTexture_readBuffer ( lxgTexturePTR tex , const lxgTextureUpdate_t * update , uint miplevel , GLenum datatype , GLenum dataformat , const struct lxgBuffer_s * buffer , uint bufferoffset ) ;
booln lxgTexture_writeData ( lxgTexturePTR tex , uint side , booln ascompressed , uint mip , GLenum datatype , GLenum dataformat , void * buffer , uint buffersize ) ;
booln lxgTexture_writeBuffer ( lxgTexturePTR tex , uint side , booln ascompressed , uint mip , GLenum datatype , GLenum dataformat , lxgBufferPTR buffer , uint bufferoffset ) ;
void lxgTexture_getSampler ( lxgTextureCPTR tex , lxgSamplerPTR sampler ) ;
void lxgTexture_boundSetSampler ( lxgTexturePTR tex , lxgSamplerCPTR sampler , flags32 what ) ;
lxVec3iCPTR lxgTexture_getMipSize ( lxgTextureCPTR tex , uint mipLevel ) ;
void lxgSampler_init ( lxgSamplerPTR sampler , lxgContextPTR ctx ) ;
void lxgSampler_deinit ( lxgSamplerPTR sampler , lxgContextPTR ctx ) ;
void lxgSampler_setAddress ( lxgSamplerPTR sampler , uint n , lxgSamplerAddress_t address ) ;
void lxgSampler_setCompare ( lxgSamplerPTR sampler , enum lxGLCompareMode_t cmp ) ;
void lxgSampler_update ( lxgSamplerPTR sampler ) ;
booln lxgRenderBuffer_init ( lxgRenderBufferPTR rb , lxgContextPTR ctx , lxgTextureChannel_t format , int width , int height , int samples ) ;
booln lxgRenderBuffer_change ( lxgRenderBufferPTR rb , lxgTextureChannel_t format , int width , int height , int samples ) ;
void lxgRenderBuffer_deinit ( lxgRenderBufferPTR rb , lxgContextPTR ctx ) ;
booln lxgTextureImage_init ( lxgTextureImagePTR img , lxgContextPTR ctx , lxgTexturePTR tex , lxgAccessMode_t acces , uint level , booln layered , int layer ) ;
typedef struct lxgDepth_s
{
    bool16 enabled ;
    bool16 write ;
    lxGLCompareMode_t func ;
}
lxgDepth_t ;
typedef struct lxgLogic_s
{
    bool32 enabled ;
    lxGLLogicOp_t op ;
}
lxgLogic_t ;
typedef enum lxgColorChannel_e
{
    LUXGFX_COLOR_RED , LUXGFX_COLOR_GREEN , LUXGFX_COLOR_BLUE , LUXGFX_COLOR_ALPHA , LUXGFX_COLORS , }
lxgColorChannel_t ;
typedef struct lxgColor_s
{
    bool32 individual ;
    bool8 write [ LUXGFX_MAX_RENDERTARGETS ] [ LUXGFX_COLORS ] ;
}
lxgColor_t ;
typedef enum lxgFaceSide_e
{
    LUXGFX_FACE_FRONT , LUXGFX_FACE_BACK , LUXGFX_FACES , }
lxgFaceSide_t ;
typedef struct lxgStencilOp_s
{
    lxGLStencilMode_t fail ;
    lxGLStencilMode_t zfail ;
    lxGLStencilMode_t zpass ;
    lxGLCompareMode_t func ;
}
lxgStencilOp_t ;
typedef struct lxgStencil_s
{
    bool8 enabled ;
    flags32 write ;
    flags32 mask ;
    uint32 refvalue ;
    lxgStencilOp_t ops [ LUXGFX_FACES ] ;
}
lxgStencil_t ;
typedef struct lxgBlendMode_s
{
    lxGLBlendWeight_t srcw ;
    lxGLBlendWeight_t dstw ;
    lxGLBlendEquation_t equ ;
}
lxgBlendMode_t ;
typedef struct lxgBlendStage_s
{
    bool32 enabled ;
    lxgBlendMode_t colormode ;
    lxgBlendMode_t alphamode ;
}
lxgBlendStage_t ;
typedef struct lxgBlend_s
{
    bool16 individual ;
    bool16 separateStages ;
    lxgBlendStage_t blends [ LUXGFX_MAX_RENDERTARGETS ] ;
}
lxgBlend_t ;
typedef struct lxgRasterizer_s
{
    bool8 cull ;
    bool8 cullfront ;
    bool8 ccw ;
    enum32 fill ;
}
lxgRasterizer_t ;
typedef struct lxgRasterState_s
{
    lxgRasterizerCPTR rasterizerObj ;
    lxgColorCPTR colorObj ;
    lxgBlendCPTR blendObj ;
    lxgDepthCPTR depthObj ;
    lxgStencilCPTR stencilObj ;
    lxgLogicCPTR logicObj ;
    lxgRasterizer_t rasterizer ;
    lxgColor_t color ;
    lxgBlend_t blend ;
    lxgDepth_t depth ;
    lxgStencil_t stencil ;
    lxgLogic_t logic ;
}
lxgRasterState_t ;
void lxgRasterizer_init ( lxgRasterizerPTR obj ) ;
void lxgRasterizer_sync ( lxgRasterizerPTR obj , lxgContextPTR ctx ) ;
void lxgColor_init ( lxgColorPTR obj ) ;
void lxgColor_sync ( lxgColorPTR obj , lxgContextPTR ctx ) ;
void lxgDepth_init ( lxgDepthPTR obj ) ;
void lxgDepth_sync ( lxgDepthPTR obj , lxgContextPTR ctx ) ;
void lxgLogic_init ( lxgLogicPTR obj ) ;
void lxgLogic_sync ( lxgLogicPTR obj , lxgContextPTR ctx ) ;
void lxgStencil_init ( lxgStencilPTR obj ) ;
void lxgStencil_sync ( lxgStencilPTR obj , lxgContextPTR ctx ) ;
void lxgBlend_init ( lxgBlendPTR obj ) ;
void lxgBlend_sync ( lxgBlendPTR obj , lxgContextPTR ctx ) ;
void lxgContext_applyColor ( lxgContextPTR ctx , lxgColorCPTR obj ) ;
void lxgContext_applyDepth ( lxgContextPTR ctx , lxgDepthCPTR obj ) ;
void lxgContext_applyLogic ( lxgContextPTR ctx , lxgLogicCPTR obj ) ;
void lxgContext_applyStencil ( lxgContextPTR ctx , lxgStencilCPTR obj ) ;
void lxgContext_applyBlend ( lxgContextPTR ctx , lxgBlendCPTR obj ) ;
void lxgContext_applyRasterizer ( lxgContextPTR ctx , lxgRasterizerCPTR obj ) ;
void lxgProgramParameter_stateColor ( lxgProgramParameterPTR param , lxgContextPTR ctx , const void * obj ) ;
void lxgProgramParameter_stateDepth ( lxgProgramParameterPTR param , lxgContextPTR ctx , const void * obj ) ;
void lxgProgramParameter_stateLogic ( lxgProgramParameterPTR param , lxgContextPTR ctx , const void * obj ) ;
void lxgProgramParameter_stateStencil ( lxgProgramParameterPTR param , lxgContextPTR ctx , const void * obj ) ;
void lxgProgramParameter_stateBlend ( lxgProgramParameterPTR param , lxgContextPTR ctx , const void * obj ) ;
void lxgProgramParameter_stateRasterizer ( lxgProgramParameterPTR param , lxgContextPTR ctx , const void * obj ) ;
typedef struct lxgFrameBounds_s
{
    int width ;
    int height ;
}
lxgFrameBounds_t ;
typedef struct lxgViewDepth_s
{
    double near ;
    double far ;
}
lxgViewDepth_t ;
typedef struct lxgViewPort_s
{
    booln scissor ;
    lxRectanglei_t scissorRect ;
    lxRectanglei_t viewRect ;
    lxgViewDepth_t depth ;
}
lxgViewPort_t ;
typedef struct lxgViewPortMrt_s
{
    uint numused ;
    flags32 scissored ;
    lxRectanglef_t bounds [ LUXGFX_MAX_RENDERTARGETS ] ;
    lxRectanglei_t scissors [ LUXGFX_MAX_RENDERTARGETS ] ;
    lxgViewDepth_t depths [ LUXGFX_MAX_RENDERTARGETS ] ;
}
lxgViewPortMrt_t ;
typedef enum lxgRenderTargetType_e
{
    LUXGFX_RENDERTARGET_DRAW , LUXGFX_RENDERTARGET_READ , LUXGFX_RENDERTARGETS }
lxgRenderTargetType_t ;
typedef struct lxgRenderAssign_s
{
    lxgTexturePTR tex ;
    lxgRenderBufferPTR rbuf ;
    uint mip ;
    uint layer ;
}
lxgRenderAssign_t ;
typedef enum lxgRenderAssignType_e
{
    LUXGFX_RENDERASSIGN_DEPTH , LUXGFX_RENDERASSIGN_STENCIL , LUXGFX_RENDERASSIGN_COLOR0 , LUXGFX_RENDERASSIGNS = LUXGFX_RENDERASSIGN_COLOR0 + LUXGFX_MAX_RENDERTARGETS , }
lxgRenderAssignType_t ;
typedef struct lxgRenderTarget_s
{
    GLuint glid ;
    lxgContextPTR ctx ;
    uint maxidx ;
    flags32 dirty ;
    lxgRenderAssign_t assigns [ LUXGFX_RENDERASSIGNS ] ;
    booln equalsized ;
    lxgFrameBounds_t bounds ;
}
lxgRenderTarget_t ;
typedef struct lxgRenderTargetBlit_s
{
    lxVec2i_t fromStart ;
    lxVec2i_t fromEnd ;
    lxVec2i_t toStart ;
    lxVec2i_t toEnd ;
}
lxgRenderTargetBlit_t ;
typedef struct lxgRenderTargetBlit_s * lxgRenderTargetBlitPTR ;
void lxgRenderTarget_init ( lxgRenderTargetPTR rt , lxgContextPTR ctx ) ;
void lxgRenderTarget_deinit ( lxgRenderTargetPTR rt , lxgContextPTR ctx ) ;
void lxgRenderTarget_applyAssigns ( lxgRenderTargetPTR rt , lxgRenderTargetType_t mode ) ;
void lxgRenderTarget_setAssign ( lxgRenderTargetPTR rt , uint assigntype , lxgRenderAssignPTR assign ) ;
booln lxgRenderTarget_checkSize ( lxgRenderTargetPTR rt ) ;
lxgFrameBoundsCPTR lxgRenderTarget_getBounds ( lxgRenderTargetPTR rt ) ;
void lxgViewPort_sync ( lxgViewPortPTR obj , lxgContextPTR ctx ) ;
void lxgViewPortMrt_sync ( lxgViewPortMrtPTR obj , lxgContextPTR ctx ) ;
void lxgContext_applyRenderTarget ( lxgContextPTR ctx , lxgRenderTargetPTR obj , lxgRenderTargetType_t type ) ;
void lxgContext_applyRenderTargetDraw ( lxgContextPTR ctx , lxgRenderTargetPTR obj , booln setViewport ) ;
void lxgContext_blitRenderTargets ( lxgContextPTR ctx , lxgRenderTargetPTR to , lxgRenderTargetPTR from , lxgRenderTargetBlitPTR update , flags32 mask , booln linearFilter ) ;
booln lxgContext_applyViewPortRect ( lxgContextPTR ctx , lxRectangleiCPTR rect ) ;
booln lxgContext_applyViewPortScissorState ( lxgContextPTR ctx , booln state ) ;
booln lxgContext_applyViewPort ( lxgContextPTR ctx , lxgViewPortPTR obj ) ;
void lxgContext_applyViewPortMrt ( lxgContextPTR ctx , lxgViewPortMrtPTR obj ) ;
typedef enum lxgProgramType_e
{
    LUXGFX_PROGRAM_NONE , LUXGFX_PROGRAM_GLSL , LUXGFX_PROGRAM_GLSLSEP , LUXGFX_PROGRAM_NV , }
lxgProgramType_t ;
typedef enum lxgProgramStage_e
{
    LUXGFX_STAGE_VERTEX , LUXGFX_STAGE_FRAGMENT , LUXGFX_STAGE_GEOMETRY , LUXGFX_STAGE_TESSCTRL , LUXGFX_STAGE_TESSEVAL , LUXGFX_STAGES , }
lxgProgramStage_t ;
typedef void ( * lxgParmeterUpdate_fn ) ( lxgProgramParameterPTR param , lxgContextPTR ctx , const void * data ) ;
typedef uint32 lxgSubroutineKey ;
typedef struct lxgProgramSubroutine_s
{
    GLuint glindex ;
    lxgSubroutineKey namekey ;
}
lxgProgramSubroutine_t ;
typedef struct lxgProgramParameter_s
{
    lxGLParameterType_t type ;
    lxgParmeterUpdate_fn func ;
    union
    {
        GLuint glid ;
        GLenum gltarget ;
        lxgProgramStage_t stage ;
        uint32 userid ;
    }
    ;
    GLuint gllocation ;
    union
    {
        struct
        {
            uint32 unit ;
            uint32 count ;
            uint32 transpose ;
        }
        uniform ;
        struct
        {
            uint32 unit ;
            int32 size ;
        }
        buffer ;
        struct
        {
            lxgProgramSubroutine_t last ;
            ushort count ;
            ushort numCompatible ;
            lxgProgramSubroutine_t * compatible ;
        }
        subroutine ;
        struct
        {
            uint32 userkey ;
            void * userdata ;
        }
        user ;
    }
    ;
    const char * name ;
}
lxgProgramParameter_t ;
typedef struct lxgStageProgram_s
{
    union
    {
        lxGLShaderType_t gltype ;
        lxGLProgramType_t gltarget ;
    }
    ;
    GLuint glid ;
    uint numSubroutines ;
    lxgContextPTR ctx ;
    lxgProgramType_t progtype ;
}
lxgStageProgram_t ;
typedef struct lxgProgram_s
{
    GLuint glid ;
    lxgProgramType_t type ;
    flags16 usedProgs ;
    bool8 isSeparable ;
    bool8 hasSubroutines ;
    union
    {
        lxgProgramPTR sepPrograms [ LUXGFX_STAGES ] ;
        lxgStageProgramPTR stagePrograms [ LUXGFX_STAGES ] ;
    }
    ;
    uint16 numSubroutines [ LUXGFX_STAGES ] ;
    lxgContextPTR ctx ;
}
lxgProgram_t ;
typedef struct lxgProgramState_s
{
    lxgProgramCPTR current ;
    flags32 dirtySubroutines ;
    uint numSubroutines [ LUXGFX_STAGES ] ;
    GLenum typeSubroutines [ LUXGFX_STAGES ] ;
    GLuint subroutines [ LUXGFX_STAGES ] [ LUXGFX_MAX_STAGE_SUBROUTINES ] ;
    lxgBufferCPTR uniform [ LUXGFX_MAX_STAGE_BUFFERS * LUXGFX_STAGES ] ;
}
lxgProgramState_t ;
booln lxGLParameterType_isValue ( lxGLParameterType_t type ) ;
booln lxGLParameterType_isTexture ( lxGLParameterType_t type ) ;
booln lxGLParameterType_isImage ( lxGLParameterType_t type ) ;
void lxgContext_applyProgram ( lxgContextPTR ctx , lxgProgramCPTR prog ) ;
void lxgContext_applyProgramParameters ( lxgContextPTR ctx , lxgProgramCPTR prog , uint num , lxgProgramParameterPTR * params , const void * * data ) ;
void lxgContext_updateProgramSubroutines ( lxgContextPTR ctx , lxgProgramCPTR prog ) ;
void lxgProgramParameter_initFunc ( lxgProgramParameterPTR param ) ;
void lxgStageProgram_init ( lxgStageProgramPTR stage , lxgContextPTR ctx , lxgProgramStage_t type ) ;
void lxgStageProgram_deinit ( lxgStageProgramPTR stage , lxgContextPTR ctx ) ;
int lxgStageProgram_compile ( lxgStageProgramPTR stage , const char * src , int len ) ;
const char * lxgStageProgram_error ( lxgStageProgramPTR stage , char * buffer , int len ) ;
void lxgProgram_init ( lxgProgramPTR prog , lxgContextPTR ctx ) ;
void lxgProgram_deinit ( lxgProgramPTR prog , lxgContextPTR ctx ) ;
void lxgProgram_setStage ( lxgProgramPTR prog , lxgProgramStage_t type , lxgStageProgramPTR stage ) ;
int lxgProgram_link ( lxgProgramPTR prog ) ;
const char * lxgProgram_log ( lxgProgramPTR prog , char * buffer , int len ) ;
int lxgProgram_getParameterCount ( lxgProgramPTR prog , int * namesSize , int * compatibleSubroutines ) ;
void lxgProgram_initParameters ( lxgProgramPTR prog , int num , lxgProgramParameter_t * params , int namesSize , char * namesBuffer ) ;
int lxgProgram_getSubroutineCount ( lxgProgramPTR prog , int * namesSize ) ;
void lxgProgram_initSubroutineParameters ( lxgProgramPTR prog , int numParams , lxgProgramParameter_t * params , int namesSize , char * namesBuffer , char * * subroutineNames , int compatibles , lxgProgramSubroutine_t * compatibleData ) ;
void lxgProgramParameter_initFuncSEP ( lxgProgramParameterPTR param , GLuint progid ) ;
void lxgProgram_initForSEP ( lxgProgramPTR prog , lxgContextPTR ctx ) ;
void lxgProgram_initSEP ( lxgProgramPTR prog , lxgContextPTR ctx ) ;
void lxgProgram_deinitSEP ( lxgProgramPTR prog , lxgContextPTR ctx ) ;
void lxgProgram_setSEP ( lxgProgramPTR prog , lxgProgramPTR stage ) ;
const char * lxgProgram_logSEP ( lxgProgramPTR prog , char * buffer , int len ) ;
void lxgProgramParameter_initFuncNV ( lxgProgramParameterPTR param , lxgProgramStage_t domain ) ;
void lxgStageProgram_initNV ( lxgStageProgramPTR stage , lxgContextPTR ctx , lxgProgramStage_t type ) ;
void lxgStageProgram_deinitNV ( lxgStageProgramPTR stage , lxgContextPTR ctx ) ;
int lxgStageProgram_compileNV ( lxgStageProgramPTR stage , const char * src , int len ) ;
const char * lxgStageProgram_errorNV ( lxgStageProgramPTR stage , char * buffer , int len ) ;
void lxgProgram_initNV ( lxgProgramPTR prog , lxgContextPTR ctx ) ;
void lxgProgram_deinitNV ( lxgProgramPTR prog , lxgContextPTR ctx ) ;
void lxgProgram_setStageNV ( lxgProgramPTR prog , lxgProgramStage_t type , lxgStageProgramPTR stage ) ;
enum lxgCapability_e
{
    LUXGFX_CAP_POINTSPRITE = 1 << 0 , LUXGFX_CAP_STENCILWRAP = 1 << 1 , LUXGFX_CAP_BLENDSEP = 1 << 2 , LUXGFX_CAP_OCCQUERY = 1 << 3 , LUXGFX_CAP_FBO = 1 << 4 , LUXGFX_CAP_FBOMS = 1 << 5 , LUXGFX_CAP_DEPTHFLOAT = 1 << 6 , LUXGFX_CAP_VBO = 1 << 7 , LUXGFX_CAP_PBO = 1 << 8 , LUXGFX_CAP_UBO = 1 << 9 , LUXGFX_CAP_TEX3D = 1 << 10 , LUXGFX_CAP_TEXRECT = 1 << 11 , LUXGFX_CAP_TEXNP2 = 1 << 12 , LUXGFX_CAP_TEXCUBEARRAY = 1 << 13 , LUXGFX_CAP_TEXS3TC = 1 << 14 , LUXGFX_CAP_TEXRGTC = 1 << 15 , LUXGFX_CAP_TEXRW = 1 << 16 , LUXGFX_CAP_BUFMAPRANGE = 1 << 17 , LUXGFX_CAP_BUFCOPY = 1 << 18 , LUXGFX_CAP_DEPTHCLAMP = 1 << 19 , LUXGFX_CAP_SM0 = 1 << 20 , LUXGFX_CAP_SM1 = 1 << 21 , LUXGFX_CAP_SM2 = 1 << 22 , LUXGFX_CAP_SM2EXT = 1 << 23 , LUXGFX_CAP_SM3 = 1 << 24 , LUXGFX_CAP_SM4 = 1 << 25 , LUXGFX_CAP_SM5 = 1 << 26 , }
;
typedef enum lxgGPUVendor_e
{
    LUXGFX_GPUVENDOR_UNKNOWN , LUXGFX_GPUVENDOR_NVIDIA , LUXGFX_GPUVENDOR_ATI , LUXGFX_GPUVENDOR_INTEL , }
lxgGPUVendor_t ;
typedef enum lxgGPUMode_e
{
    LUXGFX_GPUMODE_FIXED , LUXGFX_GPUMODE_ASM , LUXGFX_GPUMODE_HL , }
lxgGPUMode_t ;
typedef struct lxgCapabilites_s
{
    int texsize ;
    int texsize3d ;
    int texlayers ;
    int texunits ;
    int teximages ;
    int texcoords ;
    int texvtxunits ;
    float texaniso ;
    float pointsize ;
    int drawbuffers ;
    int viewports ;
    int fbosamples ;
}
lxgCapabilites_t ;
typedef struct lxgContext_s
{
    flags32 capbits ;
    lxgTexturePTR textures [ LUXGFX_MAX_TEXTURE_IMAGES ] ;
    lxgSamplerCPTR samplers [ LUXGFX_MAX_TEXTURE_IMAGES ] ;
    lxgTextureImageCPTR images [ LUXGFX_MAX_RWTEXTURE_IMAGES ] ;
    lxgRenderTargetPTR rendertargets [ LUXGFX_RENDERTARGETS ] ;
    lxgVertexState_t vertex ;
    lxgFeedbackState_t feedback ;
    lxgProgramState_t program ;
    lxgRasterState_t raster ;
    lxgViewPort_t viewport ;
    lxgFrameBounds_t framebounds ;
    lxgFrameBounds_t window ;
    lxgViewPortMrt_t viewportMRT ;
    lxgCapabilites_t capabilites ;
}
lxgContext_t ;
const char * lxgContext_init ( lxgContextPTR ctx ) ;
void lxgContext_syncRasterStates ( lxgContextPTR ctx ) ;
booln lxgContext_checkStates ( lxgContextPTR ctx ) ;
void lxgContext_clearVertexState ( lxgContextPTR ctx ) ;
void lxgContext_applyVertexAttribs ( lxgContextPTR ctx , flags32 attribs , flags32 changed ) ;
void lxgContext_applyVertexAttribsFIXED ( lxgContextPTR ctx , flags32 attribs , flags32 changed ) ;
void lxgContext_applyVertexState ( lxgContextPTR ctx ) ;
void lxgContext_applyVertexStateFIXED ( lxgContextPTR ctx ) ;
void lxgContext_applyVertexStateNV ( lxgContextPTR ctx ) ;
void lxgContext_applyVertexStateFIXEDNV ( lxgContextPTR ctx ) ;
void lxgContext_setVertexDecl ( lxgContextPTR ctx , lxgVertexDeclCPTR decl ) ;
void lxgContext_setVertexDeclStreams ( lxgContextPTR ctx , lxgVertexDeclCPTR decl , lxgStreamHostCPTR hosts ) ;
void lxgContext_setVertexStream ( lxgContextPTR ctx , uint idx , lxgStreamHostCPTR host ) ;
void lxgContext_invalidateVertexStreams ( lxgContextPTR ctx ) ;
void lxgContext_clearFeedbackState ( lxgContextPTR ctx ) ;
void lxgContext_applyFeedbackStreams ( lxgContextPTR ctx ) ;
void lxgContext_setFeedbackStreams ( lxgContextPTR ctx , lxgStreamHostCPTR hosts , int numStreams ) ;
void lxgContext_setFeedbackStream ( lxgContextPTR ctx , uint idx , lxgStreamHostCPTR host ) ;
void lxgContext_enableFeedback ( lxgContextPTR ctx , lxGLPrimitiveType_t type , int numStreams ) ;
void lxgContext_disableFeedback ( lxgContextPTR ctx ) ;
void lxgContext_clearProgramState ( lxgContextPTR ctx ) ;
void lxgContext_applyProgram ( lxgContextPTR ctx , lxgProgramCPTR prog ) ;
void lxgContext_applyProgramParameters ( lxgContextPTR ctx , lxgProgramCPTR prog , uint num , lxgProgramParameterPTR * params , const void * * data ) ;
void lxgContext_updateProgramSubroutines ( lxgContextPTR ctx , lxgProgramCPTR prog ) ;
void lxgContext_clearTextureState ( lxgContextPTR ctx ) ;
void lxgContext_setTextureSampler ( lxgContextPTR ctx , uint imageunit , flags32 what ) ;
void lxgContext_changedTextureSampler ( lxgContextPTR ctx , uint imageunit , flags32 what ) ;
void lxgContext_applyTexture ( lxgContextPTR ctx , lxgTexturePTR obj , uint imageunit ) ;
void lxgContext_applyTextures ( lxgContextPTR ctx , lxgTexturePTR * texs , uint start , uint num ) ;
void lxgContext_applySampler ( lxgContextPTR ctx , lxgSamplerCPTR obj , uint imageunit ) ;
void lxgContext_applySamplers ( lxgContextPTR ctx , lxgSamplerCPTR * samps , uint start , uint num ) ;
void lxgContext_applyTextureImages ( lxgContextPTR ctx , lxgTextureImageCPTR * imgs , uint start , uint num ) ;
void lxgContext_applyTextureImage ( lxgContextPTR ctx , lxgTextureImageCPTR img , uint imageunit ) ;
void lxgContext_clearRasterState ( lxgContextPTR ctx ) ;
void lxgContext_applyDepth ( lxgContextPTR ctx , lxgDepthCPTR obj ) ;
void lxgContext_applyLogic ( lxgContextPTR ctx , lxgLogicCPTR obj ) ;
void lxgContext_applyStencil ( lxgContextPTR ctx , lxgStencilCPTR obj ) ;
void lxgContext_applyBlend ( lxgContextPTR ctx , lxgBlendCPTR obj ) ;
void lxgContext_applyColor ( lxgContextPTR ctx , lxgColorCPTR obj ) ;
void lxgContext_applyRasterizer ( lxgContextPTR ctx , lxgRasterizerCPTR obj ) ;
void lxgContext_blitRenderTargets ( lxgContextPTR ctx , lxgRenderTargetPTR to , lxgRenderTargetPTR from , lxgRenderTargetBlitPTR update , flags32 mask , booln linearFilter ) ;
booln lxgContext_applyViewPortRect ( lxgContextPTR ctx , lxRectangleiCPTR rect ) ;
booln lxgContext_applyViewPortScissorState ( lxgContextPTR ctx , booln state ) ;
booln lxgContext_applyViewPort ( lxgContextPTR ctx , lxgViewPortPTR obj ) ;
void lxgContext_applyViewPortMrt ( lxgContextPTR ctx , lxgViewPortMrtPTR obj ) ;
void lxgContext_applyRenderTarget ( lxgContextPTR ctx , lxgRenderTargetPTR obj , lxgRenderTargetType_t type ) ;
void lxgContext_applyRenderTargetDraw ( lxgContextPTR ctx , lxgRenderTargetPTR obj , booln setViewport ) ;
void lxgContext_checkedBlend ( lxgContextPTR ctx , lxgBlendCPTR obj ) ;
void lxgContext_checkedColor ( lxgContextPTR ctx , lxgColorCPTR obj ) ;
void lxgContext_checkedDepth ( lxgContextPTR ctx , lxgDepthCPTR obj ) ;
void lxgContext_checkedLogic ( lxgContextPTR ctx , lxgLogicCPTR obj ) ;
void lxgContext_checkedStencil ( lxgContextPTR ctx , lxgStencilCPTR obj ) ;
void lxgContext_checkedRasterizer ( lxgContextPTR ctx , lxgRasterizerCPTR obj ) ;
void lxgContext_checkedTexture ( lxgContextPTR ctx , lxgTexturePTR tex , uint imageunit ) ;
void lxgContext_checkedSampler ( lxgContextPTR ctx , lxgSamplerCPTR samp , uint imageunit ) ;
void lxgContext_checkedTextureImage ( lxgContextPTR ctx , lxgTextureImageCPTR img , uint imageunit ) ;
void lxgContext_checkedTextures ( lxgContextPTR ctx , lxgTexturePTR * texs , uint start , uint num ) ;
void lxgContext_checkedSamplers ( lxgContextPTR ctx , lxgSamplerCPTR * samps , uint start , uint num ) ;
void lxgContext_checkedTextureImages ( lxgContextPTR ctx , lxgTextureImageCPTR * imgs , uint start , uint num ) ;
void lxgContext_checkedRenderFlag ( lxgContextPTR ctx , flags32 needed ) ;
void lxgContext_checkedVertexDecl ( lxgContextPTR ctx , lxgVertexDeclCPTR decl ) ;
void lxgContext_checkedVertexAttrib ( lxgContextPTR ctx , flags32 needed ) ;
void lxgContext_checkedVertexAttribFIXED ( lxgContextPTR ctx , flags32 needed ) ;
void lxgContext_checkedRenderTarget ( lxgContextPTR ctx , lxgRenderTargetPTR rt , lxgRenderTargetType_t type ) ;
void lxgContext_checkedProgram ( lxgContextPTR ctx , lxgProgramPTR prog ) ;
void lxgContext_checkedVertex ( lxgContextPTR ctx ) ;
void lxgContext_checkedVertexNV ( lxgContextPTR ctx ) ;
void lxgContext_checkedVertexFIXED ( lxgContextPTR ctx ) ;
void lxgContext_checkedVertexFIXEDNV ( lxgContextPTR ctx ) ;
void lxgContext_checkedViewPortScissor ( lxgContextPTR ctx , lxRectangleiCPTR rect ) ;
void lxgContext_checkedTextureSampler ( lxgContextPTR ctx , uint imageunit ) ;
booln lxgContext_setProgramBuffer ( lxgContextPTR ctx , uint idx , lxgBufferCPTR buffer ) ;
]]

return ffi.load("luxbackend")
