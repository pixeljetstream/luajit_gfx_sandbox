local ffi = require 'ffi'
ffi.cdef [[
typedef __int64 GLint64EXT;
typedef unsigned __int64 GLuint64EXT;
typedef GLint64EXT  GLint64;
typedef GLuint64EXT GLuint64;
typedef struct __GLsync *GLsync;
typedef char GLchar;
typedef double GLclampd;
typedef double GLdouble;
typedef float GLclampf;
typedef float GLfloat;
typedef int GLint;
typedef int GLsizei;
typedef short GLshort;
typedef signed char GLbyte;
typedef unsigned char GLboolean;
typedef unsigned char GLubyte;
typedef unsigned int GLbitfield;
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef unsigned long GLulong;
typedef unsigned short GLushort;
typedef void GLvoid;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef struct _cl_context *cl_context;
typedef struct _cl_event *cl_event;
typedef unsigned short GLhalf;
typedef GLintptr GLvdpauSurfaceNV;

enum {
GL_AUX0 = 0x0409,
GL_AUX1 = 0x040A,
GL_AUX2 = 0x040B,
GL_AUX3 = 0x040C,
GL_EXP2 = 0x0801,
GL_MAP1_COLOR_4 = 0x0D90,
GL_MAP1_TEXTURE_COORD_1 = 0x0D93,
GL_MAP1_TEXTURE_COORD_2 = 0x0D94,
GL_MAP1_TEXTURE_COORD_3 = 0x0D95,
GL_MAP1_TEXTURE_COORD_4 = 0x0D96,
GL_MAP1_VERTEX_3 = 0x0D97,
GL_MAP1_VERTEX_4 = 0x0D98,
GL_MAP2_COLOR_4 = 0x0DB0,
GL_MAP2_TEXTURE_COORD_1 = 0x0DB3,
GL_MAP2_TEXTURE_COORD_2 = 0x0DB4,
GL_MAP2_TEXTURE_COORD_3 = 0x0DB5,
GL_MAP2_TEXTURE_COORD_4 = 0x0DB6,
GL_MAP2_VERTEX_3 = 0x0DB7,
GL_MAP2_VERTEX_4 = 0x0DB8,
GL_R3_G3_B2 = 0x2A10,
GL_CLIP_PLANE0 = 0x3000,
GL_CLIP_PLANE1 = 0x3001,
GL_CLIP_PLANE2 = 0x3002,
GL_CLIP_PLANE3 = 0x3003,
GL_CLIP_PLANE4 = 0x3004,
GL_CLIP_PLANE5 = 0x3005,
GL_LIGHT0 = 0x4000,
GL_LIGHT1 = 0x4001,
GL_LIGHT2 = 0x4002,
GL_LIGHT3 = 0x4003,
GL_LIGHT4 = 0x4004,
GL_LIGHT5 = 0x4005,
GL_LIGHT6 = 0x4006,
GL_LIGHT7 = 0x4007,
GL_ALPHA4 = 0x803B,
GL_ALPHA8 = 0x803C,
GL_ALPHA12 = 0x803D,
GL_ALPHA16 = 0x803E,
GL_LUMINANCE4 = 0x803F,
GL_LUMINANCE8 = 0x8040,
GL_LUMINANCE12 = 0x8041,
GL_LUMINANCE16 = 0x8042,
GL_LUMINANCE4_ALPHA4 = 0x8043,
GL_LUMINANCE6_ALPHA2 = 0x8044,
GL_LUMINANCE8_ALPHA8 = 0x8045,
GL_LUMINANCE12_ALPHA4 = 0x8046,
GL_LUMINANCE12_ALPHA12 = 0x8047,
GL_LUMINANCE16_ALPHA16 = 0x8048,
GL_INTENSITY4 = 0x804A,
GL_INTENSITY8 = 0x804B,
GL_INTENSITY12 = 0x804C,
GL_INTENSITY16 = 0x804D,
GL_RGB4 = 0x804F,
GL_RGB5 = 0x8050,
GL_RGB8 = 0x8051,
GL_RGB10 = 0x8052,
GL_RGB12 = 0x8053,
GL_RGB16 = 0x8054,
GL_RGBA2 = 0x8055,
GL_RGBA4 = 0x8056,
GL_RGB5_A1 = 0x8057,
GL_RGBA8 = 0x8058,
GL_RGB10_A2 = 0x8059,
GL_RGBA12 = 0x805A,
GL_RGBA16 = 0x805B,
GL_UNSIGNED_BYTE_3_3_2 = 0x8032,
GL_UNSIGNED_SHORT_4_4_4_4 = 0x8033,
GL_UNSIGNED_SHORT_5_5_5_1 = 0x8034,
GL_UNSIGNED_INT_8_8_8_8 = 0x8035,
GL_UNSIGNED_INT_10_10_10_2 = 0x8036,
GL_UNSIGNED_SHORT_5_6_5 = 0x8363,
GL_TEXTURE0 = 0x84C0,
GL_TEXTURE1 = 0x84C1,
GL_TEXTURE2 = 0x84C2,
GL_TEXTURE3 = 0x84C3,
GL_TEXTURE4 = 0x84C4,
GL_TEXTURE5 = 0x84C5,
GL_TEXTURE6 = 0x84C6,
GL_TEXTURE7 = 0x84C7,
GL_TEXTURE8 = 0x84C8,
GL_TEXTURE9 = 0x84C9,
GL_TEXTURE10 = 0x84CA,
GL_TEXTURE11 = 0x84CB,
GL_TEXTURE12 = 0x84CC,
GL_TEXTURE13 = 0x84CD,
GL_TEXTURE14 = 0x84CE,
GL_TEXTURE15 = 0x84CF,
GL_TEXTURE16 = 0x84D0,
GL_TEXTURE17 = 0x84D1,
GL_TEXTURE18 = 0x84D2,
GL_TEXTURE19 = 0x84D3,
GL_TEXTURE20 = 0x84D4,
GL_TEXTURE21 = 0x84D5,
GL_TEXTURE22 = 0x84D6,
GL_TEXTURE23 = 0x84D7,
GL_TEXTURE24 = 0x84D8,
GL_TEXTURE25 = 0x84D9,
GL_TEXTURE26 = 0x84DA,
GL_TEXTURE27 = 0x84DB,
GL_TEXTURE28 = 0x84DC,
GL_TEXTURE29 = 0x84DD,
GL_TEXTURE30 = 0x84DE,
GL_TEXTURE31 = 0x84DF,
GL_DEPTH_COMPONENT16 = 0x81A5,
GL_DEPTH_COMPONENT24 = 0x81A6,
GL_DEPTH_COMPONENT32 = 0x81A7,
GL_DRAW_BUFFER0 = 0x8825,
GL_DRAW_BUFFER1 = 0x8826,
GL_DRAW_BUFFER2 = 0x8827,
GL_DRAW_BUFFER3 = 0x8828,
GL_DRAW_BUFFER4 = 0x8829,
GL_DRAW_BUFFER5 = 0x882A,
GL_DRAW_BUFFER6 = 0x882B,
GL_DRAW_BUFFER7 = 0x882C,
GL_DRAW_BUFFER8 = 0x882D,
GL_DRAW_BUFFER9 = 0x882E,
GL_DRAW_BUFFER10 = 0x882F,
GL_DRAW_BUFFER11 = 0x8830,
GL_DRAW_BUFFER12 = 0x8831,
GL_DRAW_BUFFER13 = 0x8832,
GL_DRAW_BUFFER14 = 0x8833,
GL_DRAW_BUFFER15 = 0x8834,
GL_FLOAT_VEC2 = 0x8B50,
GL_FLOAT_VEC3 = 0x8B51,
GL_FLOAT_VEC4 = 0x8B52,
GL_INT_VEC2 = 0x8B53,
GL_INT_VEC3 = 0x8B54,
GL_INT_VEC4 = 0x8B55,
GL_BOOL_VEC2 = 0x8B57,
GL_BOOL_VEC3 = 0x8B58,
GL_BOOL_VEC4 = 0x8B59,
GL_FLOAT_MAT2 = 0x8B5A,
GL_FLOAT_MAT3 = 0x8B5B,
GL_FLOAT_MAT4 = 0x8B5C,
GL_FLOAT_MAT2x3 = 0x8B65,
GL_FLOAT_MAT2x4 = 0x8B66,
GL_FLOAT_MAT3x2 = 0x8B67,
GL_FLOAT_MAT3x4 = 0x8B68,
GL_FLOAT_MAT4x2 = 0x8B69,
GL_FLOAT_MAT4x3 = 0x8B6A,
GL_SRGB8 = 0x8C41,
GL_SRGB8_ALPHA8 = 0x8C43,
GL_SLUMINANCE8_ALPHA8 = 0x8C45,
GL_SLUMINANCE8 = 0x8C47,
GL_CLIP_DISTANCE5 = 0x3005,
GL_CLIP_DISTANCE1 = 0x3001,
GL_CLIP_DISTANCE3 = 0x3003,
GL_CLIP_DISTANCE0 = 0x3000,
GL_CLIP_DISTANCE4 = 0x3004,
GL_CLIP_DISTANCE2 = 0x3002,
GL_RGB9_E5 = 0x8C3D,
GL_UNSIGNED_INT_VEC2 = 0x8DC6,
GL_UNSIGNED_INT_VEC3 = 0x8DC7,
GL_UNSIGNED_INT_VEC4 = 0x8DC8,
GL_DEPTH32F_STENCIL8 = 0x8CAD,
GL_UNSIGNED_INT_24_8 = 0x84FA,
GL_DEPTH24_STENCIL8 = 0x88F0,
GL_COLOR_ATTACHMENT0 = 0x8CE0,
GL_COLOR_ATTACHMENT1 = 0x8CE1,
GL_COLOR_ATTACHMENT2 = 0x8CE2,
GL_COLOR_ATTACHMENT3 = 0x8CE3,
GL_COLOR_ATTACHMENT4 = 0x8CE4,
GL_COLOR_ATTACHMENT5 = 0x8CE5,
GL_COLOR_ATTACHMENT6 = 0x8CE6,
GL_COLOR_ATTACHMENT7 = 0x8CE7,
GL_COLOR_ATTACHMENT8 = 0x8CE8,
GL_COLOR_ATTACHMENT9 = 0x8CE9,
GL_COLOR_ATTACHMENT10 = 0x8CEA,
GL_COLOR_ATTACHMENT11 = 0x8CEB,
GL_COLOR_ATTACHMENT12 = 0x8CEC,
GL_COLOR_ATTACHMENT13 = 0x8CED,
GL_COLOR_ATTACHMENT14 = 0x8CEE,
GL_COLOR_ATTACHMENT15 = 0x8CEF,
GL_STENCIL_INDEX1 = 0x8D46,
GL_STENCIL_INDEX4 = 0x8D47,
GL_STENCIL_INDEX8 = 0x8D48,
GL_STENCIL_INDEX16 = 0x8D49,
GL_DOUBLE_MAT2 = 0x8F46,
GL_DOUBLE_MAT3 = 0x8F47,
GL_DOUBLE_MAT4 = 0x8F48,
GL_DOUBLE_MAT2x3 = 0x8F49,
GL_DOUBLE_MAT2x4 = 0x8F4A,
GL_DOUBLE_MAT3x2 = 0x8F4B,
GL_DOUBLE_MAT3x4 = 0x8F4C,
GL_DOUBLE_MAT4x2 = 0x8F4D,
GL_DOUBLE_MAT4x3 = 0x8F4E,
GL_DOUBLE_VEC2 = 0x8FFC,
GL_DOUBLE_VEC3 = 0x8FFD,
GL_DOUBLE_VEC4 = 0x8FFE,
GL_COMPRESSED_RED_RGTC1 = 0x8DBB,
GL_COMPRESSED_SIGNED_RED_RGTC1 = 0x8DBC,
GL_COMPRESSED_RG_RGTC2 = 0x8DBD,
GL_COMPRESSED_SIGNED_RG_RGTC2 = 0x8DBE,
GL_R8 = 0x8229,
GL_R16 = 0x822A,
GL_RG8 = 0x822B,
GL_RG16 = 0x822C,
//GL_DOUBLE_MAT2 = 0x8F46,
//GL_DOUBLE_MAT3 = 0x8F47,
//GL_DOUBLE_MAT4 = 0x8F48,
//GL_DOUBLE_VEC2 = 0x8FFC,
//GL_DOUBLE_VEC3 = 0x8FFD,
//GL_DOUBLE_VEC4 = 0x8FFE,
GL_QUERY_WAIT_NV = 0x8E13,
GL_QUERY_NO_WAIT_NV = 0x8E14,
GL_QUERY_BY_REGION_WAIT_NV = 0x8E15,
GL_QUERY_BY_REGION_NO_WAIT_NV = 0x8E16,
GL_DEPTH_STENCIL_TO_RGBA_NV = 0x886E,
GL_DEPTH_STENCIL_TO_BGRA_NV = 0x886F,
GL_DEPTH_COMPONENT32F_NV = 0x8DAB,
GL_DEPTH32F_STENCIL8_NV = 0x8DAC,
GL_FLOAT_32_UNSIGNED_INT_24_8_REV_NV = 0x8DAD,
GL_DEPTH_BUFFER_FLOAT_MODE_NV = 0x8DAF,
GL_DEPTH_CLAMP_NV = 0x864F,
GL_SAMPLE_COUNT_BITS_NV = 0x8864,
GL_CURRENT_SAMPLE_COUNT_QUERY_NV = 0x8865,
GL_QUERY_RESULT_NV = 0x8866,
GL_QUERY_RESULT_AVAILABLE_NV = 0x8867,
GL_SAMPLE_COUNT_NV = 0x8914,
GL_EVAL_2D_NV = 0x86C0,
GL_EVAL_TRIANGULAR_2D_NV = 0x86C1,
GL_MAP_TESSELLATION_NV = 0x86C2,
GL_MAP_ATTRIB_U_ORDER_NV = 0x86C3,
GL_MAP_ATTRIB_V_ORDER_NV = 0x86C4,
GL_EVAL_FRACTIONAL_TESSELLATION_NV = 0x86C5,
GL_EVAL_VERTEX_ATTRIB0_NV = 0x86C6,
GL_EVAL_VERTEX_ATTRIB1_NV = 0x86C7,
GL_EVAL_VERTEX_ATTRIB2_NV = 0x86C8,
GL_EVAL_VERTEX_ATTRIB3_NV = 0x86C9,
GL_EVAL_VERTEX_ATTRIB4_NV = 0x86CA,
GL_EVAL_VERTEX_ATTRIB5_NV = 0x86CB,
GL_EVAL_VERTEX_ATTRIB6_NV = 0x86CC,
GL_EVAL_VERTEX_ATTRIB7_NV = 0x86CD,
GL_EVAL_VERTEX_ATTRIB8_NV = 0x86CE,
GL_EVAL_VERTEX_ATTRIB9_NV = 0x86CF,
GL_EVAL_VERTEX_ATTRIB10_NV = 0x86D0,
GL_EVAL_VERTEX_ATTRIB11_NV = 0x86D1,
GL_EVAL_VERTEX_ATTRIB12_NV = 0x86D2,
GL_EVAL_VERTEX_ATTRIB13_NV = 0x86D3,
GL_EVAL_VERTEX_ATTRIB14_NV = 0x86D4,
GL_EVAL_VERTEX_ATTRIB15_NV = 0x86D5,
GL_MAX_MAP_TESSELLATION_NV = 0x86D6,
GL_MAX_RATIONAL_EVAL_ORDER_NV = 0x86D7,
GL_SAMPLE_POSITION_NV = 0x8E50,
GL_SAMPLE_MASK_NV = 0x8E51,
GL_SAMPLE_MASK_VALUE_NV = 0x8E52,
GL_TEXTURE_BINDING_RENDERBUFFER_NV = 0x8E53,
GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV = 0x8E54,
GL_TEXTURE_RENDERBUFFER_NV = 0x8E55,
GL_SAMPLER_RENDERBUFFER_NV = 0x8E56,
GL_INT_SAMPLER_RENDERBUFFER_NV = 0x8E57,
GL_UNSIGNED_INT_SAMPLER_RENDERBUFFER_NV = 0x8E58,
GL_MAX_SAMPLE_MASK_WORDS_NV = 0x8E59,
GL_ALL_COMPLETED_NV = 0x84F2,
GL_FENCE_STATUS_NV = 0x84F3,
GL_FENCE_CONDITION_NV = 0x84F4,
GL_FLOAT_R_NV = 0x8880,
GL_FLOAT_RG_NV = 0x8881,
GL_FLOAT_RGB_NV = 0x8882,
GL_FLOAT_RGBA_NV = 0x8883,
GL_FLOAT_R16_NV = 0x8884,
GL_FLOAT_R32_NV = 0x8885,
GL_FLOAT_RG16_NV = 0x8886,
GL_FLOAT_RG32_NV = 0x8887,
GL_FLOAT_RGB16_NV = 0x8888,
GL_FLOAT_RGB32_NV = 0x8889,
GL_FLOAT_RGBA16_NV = 0x888A,
GL_FLOAT_RGBA32_NV = 0x888B,
GL_TEXTURE_FLOAT_COMPONENTS_NV = 0x888C,
GL_FLOAT_CLEAR_COLOR_VALUE_NV = 0x888D,
GL_FLOAT_RGBA_MODE_NV = 0x888E,
GL_FOG_DISTANCE_MODE_NV = 0x855A,
GL_EYE_RADIAL_NV = 0x855B,
GL_EYE_PLANE_ABSOLUTE_NV = 0x855C,
GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV = 0x8868,
GL_FRAGMENT_PROGRAM_NV = 0x8870,
GL_MAX_TEXTURE_COORDS_NV = 0x8871,
GL_MAX_TEXTURE_IMAGE_UNITS_NV = 0x8872,
GL_FRAGMENT_PROGRAM_BINDING_NV = 0x8873,
GL_PROGRAM_ERROR_STRING_NV = 0x8874,
GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV = 0x88F4,
GL_MAX_PROGRAM_CALL_DEPTH_NV = 0x88F5,
GL_MAX_PROGRAM_IF_DEPTH_NV = 0x88F6,
GL_MAX_PROGRAM_LOOP_DEPTH_NV = 0x88F7,
GL_MAX_PROGRAM_LOOP_COUNT_NV = 0x88F8,
GL_RENDERBUFFER_COVERAGE_SAMPLES_NV = 0x8CAB,
GL_RENDERBUFFER_COLOR_SAMPLES_NV = 0x8E10,
GL_MAX_MULTISAMPLE_COVERAGE_MODES_NV = 0x8E11,
GL_MULTISAMPLE_COVERAGE_MODES_NV = 0x8E12,
GL_GEOMETRY_PROGRAM_NV = 0x8C26,
GL_MAX_PROGRAM_OUTPUT_VERTICES_NV = 0x8C27,
GL_MAX_PROGRAM_TOTAL_OUTPUT_COMPONENTS_NV = 0x8C28,
GL_MIN_PROGRAM_TEXEL_OFFSET_NV = 0x8904,
GL_MAX_PROGRAM_TEXEL_OFFSET_NV = 0x8905,
GL_PROGRAM_ATTRIB_COMPONENTS_NV = 0x8906,
GL_PROGRAM_RESULT_COMPONENTS_NV = 0x8907,
GL_MAX_PROGRAM_ATTRIB_COMPONENTS_NV = 0x8908,
GL_MAX_PROGRAM_RESULT_COMPONENTS_NV = 0x8909,
GL_MAX_PROGRAM_GENERIC_ATTRIBS_NV = 0x8DA5,
GL_MAX_PROGRAM_GENERIC_RESULTS_NV = 0x8DA6,
GL_MAX_GEOMETRY_PROGRAM_INVOCATIONS_NV = 0x8E5A,
GL_MIN_FRAGMENT_INTERPOLATION_OFFSET_NV = 0x8E5B,
GL_MAX_FRAGMENT_INTERPOLATION_OFFSET_NV = 0x8E5C,
GL_FRAGMENT_PROGRAM_INTERPOLATION_OFFSET_BITS_NV = 0x8E5D,
GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_NV = 0x8E5E,
GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_NV = 0x8E5F,
GL_INT64_NV = 0x140E,
GL_UNSIGNED_INT64_NV = 0x140F,
GL_INT8_NV = 0x8FE0,
GL_INT8_VEC2_NV = 0x8FE1,
GL_INT8_VEC3_NV = 0x8FE2,
GL_INT8_VEC4_NV = 0x8FE3,
GL_INT16_NV = 0x8FE4,
GL_INT16_VEC2_NV = 0x8FE5,
GL_INT16_VEC3_NV = 0x8FE6,
GL_INT16_VEC4_NV = 0x8FE7,
GL_INT64_VEC2_NV = 0x8FE9,
GL_INT64_VEC3_NV = 0x8FEA,
GL_INT64_VEC4_NV = 0x8FEB,
GL_UNSIGNED_INT8_NV = 0x8FEC,
GL_UNSIGNED_INT8_VEC2_NV = 0x8FED,
GL_UNSIGNED_INT8_VEC3_NV = 0x8FEE,
GL_UNSIGNED_INT8_VEC4_NV = 0x8FEF,
GL_UNSIGNED_INT16_NV = 0x8FF0,
GL_UNSIGNED_INT16_VEC2_NV = 0x8FF1,
GL_UNSIGNED_INT16_VEC3_NV = 0x8FF2,
GL_UNSIGNED_INT16_VEC4_NV = 0x8FF3,
GL_UNSIGNED_INT64_VEC2_NV = 0x8FF5,
GL_UNSIGNED_INT64_VEC3_NV = 0x8FF6,
GL_UNSIGNED_INT64_VEC4_NV = 0x8FF7,
GL_FLOAT16_NV = 0x8FF8,
GL_FLOAT16_VEC2_NV = 0x8FF9,
GL_FLOAT16_VEC3_NV = 0x8FFA,
GL_FLOAT16_VEC4_NV = 0x8FFB,
GL_HALF_FLOAT_NV = 0x140B,
GL_MAX_SHININESS_NV = 0x8504,
GL_MAX_SPOT_EXPONENT_NV = 0x8505,
GL_COVERAGE_SAMPLES_NV = 0x80A9,
GL_COLOR_SAMPLES_NV = 0x8E20,
GL_MULTISAMPLE_FILTER_HINT_NV = 0x8534,
GL_PIXEL_COUNTER_BITS_NV = 0x8864,
GL_CURRENT_OCCLUSION_QUERY_ID_NV = 0x8865,
GL_PIXEL_COUNT_NV = 0x8866,
GL_PIXEL_COUNT_AVAILABLE_NV = 0x8867,
GL_DEPTH_STENCIL_NV = 0x84F9,
GL_UNSIGNED_INT_24_8_NV = 0x84FA,
GL_MAX_PROGRAM_PARAMETER_BUFFER_BINDINGS_NV = 0x8DA0,
GL_MAX_PROGRAM_PARAMETER_BUFFER_SIZE_NV = 0x8DA1,
GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV = 0x8DA2,
GL_GEOMETRY_PROGRAM_PARAMETER_BUFFER_NV = 0x8DA3,
GL_FRAGMENT_PROGRAM_PARAMETER_BUFFER_NV = 0x8DA4,
GL_WRITE_PIXEL_DATA_RANGE_NV = 0x8878,
GL_READ_PIXEL_DATA_RANGE_NV = 0x8879,
GL_WRITE_PIXEL_DATA_RANGE_LENGTH_NV = 0x887A,
GL_READ_PIXEL_DATA_RANGE_LENGTH_NV = 0x887B,
GL_WRITE_PIXEL_DATA_RANGE_POINTER_NV = 0x887C,
GL_READ_PIXEL_DATA_RANGE_POINTER_NV = 0x887D,
GL_POINT_SPRITE_NV = 0x8861,
GL_COORD_REPLACE_NV = 0x8862,
GL_POINT_SPRITE_R_MODE_NV = 0x8863,
GL_FRAME_NV = 0x8E26,
GL_FIELDS_NV = 0x8E27,
GL_CURRENT_TIME_NV = 0x8E28,
GL_NUM_FILL_STREAMS_NV = 0x8E29,
GL_PRESENT_TIME_NV = 0x8E2A,
GL_PRESENT_DURATION_NV = 0x8E2B,
GL_PRIMITIVE_RESTART_NV = 0x8558,
GL_PRIMITIVE_RESTART_INDEX_NV = 0x8559,
GL_REGISTER_COMBINERS_NV = 0x8522,
GL_VARIABLE_A_NV = 0x8523,
GL_VARIABLE_B_NV = 0x8524,
GL_VARIABLE_C_NV = 0x8525,
GL_VARIABLE_D_NV = 0x8526,
GL_VARIABLE_E_NV = 0x8527,
GL_VARIABLE_F_NV = 0x8528,
GL_VARIABLE_G_NV = 0x8529,
GL_CONSTANT_COLOR0_NV = 0x852A,
GL_CONSTANT_COLOR1_NV = 0x852B,
GL_PRIMARY_COLOR_NV = 0x852C,
GL_SECONDARY_COLOR_NV = 0x852D,
GL_SPARE0_NV = 0x852E,
GL_SPARE1_NV = 0x852F,
GL_DISCARD_NV = 0x8530,
GL_E_TIMES_F_NV = 0x8531,
GL_SPARE0_PLUS_SECONDARY_COLOR_NV = 0x8532,
GL_UNSIGNED_IDENTITY_NV = 0x8536,
GL_UNSIGNED_INVERT_NV = 0x8537,
GL_EXPAND_NORMAL_NV = 0x8538,
GL_EXPAND_NEGATE_NV = 0x8539,
GL_HALF_BIAS_NORMAL_NV = 0x853A,
GL_HALF_BIAS_NEGATE_NV = 0x853B,
GL_SIGNED_IDENTITY_NV = 0x853C,
GL_SIGNED_NEGATE_NV = 0x853D,
GL_SCALE_BY_TWO_NV = 0x853E,
GL_SCALE_BY_FOUR_NV = 0x853F,
GL_SCALE_BY_ONE_HALF_NV = 0x8540,
GL_BIAS_BY_NEGATIVE_ONE_HALF_NV = 0x8541,
GL_COMBINER_INPUT_NV = 0x8542,
GL_COMBINER_MAPPING_NV = 0x8543,
GL_COMBINER_COMPONENT_USAGE_NV = 0x8544,
GL_COMBINER_AB_DOT_PRODUCT_NV = 0x8545,
GL_COMBINER_CD_DOT_PRODUCT_NV = 0x8546,
GL_COMBINER_MUX_SUM_NV = 0x8547,
GL_COMBINER_SCALE_NV = 0x8548,
GL_COMBINER_BIAS_NV = 0x8549,
GL_COMBINER_AB_OUTPUT_NV = 0x854A,
GL_COMBINER_CD_OUTPUT_NV = 0x854B,
GL_COMBINER_SUM_OUTPUT_NV = 0x854C,
GL_MAX_GENERAL_COMBINERS_NV = 0x854D,
GL_NUM_GENERAL_COMBINERS_NV = 0x854E,
GL_COLOR_SUM_CLAMP_NV = 0x854F,
GL_COMBINER0_NV = 0x8550,
GL_COMBINER1_NV = 0x8551,
GL_COMBINER2_NV = 0x8552,
GL_COMBINER3_NV = 0x8553,
GL_COMBINER4_NV = 0x8554,
GL_COMBINER5_NV = 0x8555,
GL_COMBINER6_NV = 0x8556,
GL_COMBINER7_NV = 0x8557,
GL_PER_STAGE_CONSTANTS_NV = 0x8535,
GL_BUFFER_GPU_ADDRESS_NV = 0x8F1D,
GL_GPU_ADDRESS_NV = 0x8F34,
GL_MAX_SHADER_BUFFER_ADDRESS_NV = 0x8F35,
GL_MAX_PROGRAM_PATCH_ATTRIBS_NV = 0x86D8,
GL_TESS_CONTROL_PROGRAM_NV = 0x891E,
GL_TESS_EVALUATION_PROGRAM_NV = 0x891F,
GL_TESS_CONTROL_PROGRAM_PARAMETER_BUFFER_NV = 0x8C74,
GL_TESS_EVALUATION_PROGRAM_PARAMETER_BUFFER_NV = 0x8C75,
GL_EMBOSS_LIGHT_NV = 0x855D,
GL_EMBOSS_CONSTANT_NV = 0x855E,
GL_EMBOSS_MAP_NV = 0x855F,
GL_NORMAL_MAP_NV = 0x8511,
GL_REFLECTION_MAP_NV = 0x8512,
GL_COMBINE4_NV = 0x8503,
GL_SOURCE3_RGB_NV = 0x8583,
GL_SOURCE3_ALPHA_NV = 0x858B,
GL_OPERAND3_RGB_NV = 0x8593,
GL_OPERAND3_ALPHA_NV = 0x859B,
GL_TEXTURE_UNSIGNED_REMAP_MODE_NV = 0x888F,
GL_TEXTURE_RECTANGLE_NV = 0x84F5,
GL_TEXTURE_BINDING_RECTANGLE_NV = 0x84F6,
GL_PROXY_TEXTURE_RECTANGLE_NV = 0x84F7,
GL_MAX_RECTANGLE_TEXTURE_SIZE_NV = 0x84F8,
GL_OFFSET_TEXTURE_RECTANGLE_NV = 0x864C,
GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV = 0x864D,
GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV = 0x864E,
GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV = 0x86D9,
GL_UNSIGNED_INT_S8_S8_8_8_NV = 0x86DA,
GL_UNSIGNED_INT_8_8_S8_S8_REV_NV = 0x86DB,
GL_DSDT_MAG_INTENSITY_NV = 0x86DC,
GL_SHADER_CONSISTENT_NV = 0x86DD,
GL_TEXTURE_SHADER_NV = 0x86DE,
GL_SHADER_OPERATION_NV = 0x86DF,
GL_CULL_MODES_NV = 0x86E0,
GL_OFFSET_TEXTURE_2D_MATRIX_NV = 0x86E1,
GL_OFFSET_TEXTURE_MATRIX_NV = 0x86E1,
GL_OFFSET_TEXTURE_2D_SCALE_NV = 0x86E2,
GL_OFFSET_TEXTURE_SCALE_NV = 0x86E2,
GL_OFFSET_TEXTURE_BIAS_NV = 0x86E3,
GL_OFFSET_TEXTURE_2D_BIAS_NV = 0x86E3,
GL_PREVIOUS_TEXTURE_INPUT_NV = 0x86E4,
GL_CONST_EYE_NV = 0x86E5,
GL_PASS_THROUGH_NV = 0x86E6,
GL_CULL_FRAGMENT_NV = 0x86E7,
GL_OFFSET_TEXTURE_2D_NV = 0x86E8,
GL_DEPENDENT_AR_TEXTURE_2D_NV = 0x86E9,
GL_DEPENDENT_GB_TEXTURE_2D_NV = 0x86EA,
GL_DOT_PRODUCT_NV = 0x86EC,
GL_DOT_PRODUCT_DEPTH_REPLACE_NV = 0x86ED,
GL_DOT_PRODUCT_TEXTURE_2D_NV = 0x86EE,
GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV = 0x86F0,
GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV = 0x86F1,
GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV = 0x86F2,
GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV = 0x86F3,
GL_HILO_NV = 0x86F4,
GL_DSDT_NV = 0x86F5,
GL_DSDT_MAG_NV = 0x86F6,
GL_DSDT_MAG_VIB_NV = 0x86F7,
GL_HILO16_NV = 0x86F8,
GL_SIGNED_HILO_NV = 0x86F9,
GL_SIGNED_HILO16_NV = 0x86FA,
GL_SIGNED_RGBA_NV = 0x86FB,
GL_SIGNED_RGBA8_NV = 0x86FC,
GL_SIGNED_RGB_NV = 0x86FE,
GL_SIGNED_RGB8_NV = 0x86FF,
GL_SIGNED_LUMINANCE_NV = 0x8701,
GL_SIGNED_LUMINANCE8_NV = 0x8702,
GL_SIGNED_LUMINANCE_ALPHA_NV = 0x8703,
GL_SIGNED_LUMINANCE8_ALPHA8_NV = 0x8704,
GL_SIGNED_ALPHA_NV = 0x8705,
GL_SIGNED_ALPHA8_NV = 0x8706,
GL_SIGNED_INTENSITY_NV = 0x8707,
GL_SIGNED_INTENSITY8_NV = 0x8708,
GL_DSDT8_NV = 0x8709,
GL_DSDT8_MAG8_NV = 0x870A,
GL_DSDT8_MAG8_INTENSITY8_NV = 0x870B,
GL_SIGNED_RGB_UNSIGNED_ALPHA_NV = 0x870C,
GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV = 0x870D,
GL_HI_SCALE_NV = 0x870E,
GL_LO_SCALE_NV = 0x870F,
GL_DS_SCALE_NV = 0x8710,
GL_DT_SCALE_NV = 0x8711,
GL_MAGNITUDE_SCALE_NV = 0x8712,
GL_VIBRANCE_SCALE_NV = 0x8713,
GL_HI_BIAS_NV = 0x8714,
GL_LO_BIAS_NV = 0x8715,
GL_DS_BIAS_NV = 0x8716,
GL_DT_BIAS_NV = 0x8717,
GL_MAGNITUDE_BIAS_NV = 0x8718,
GL_VIBRANCE_BIAS_NV = 0x8719,
GL_TEXTURE_BORDER_VALUES_NV = 0x871A,
GL_TEXTURE_HI_SIZE_NV = 0x871B,
GL_TEXTURE_LO_SIZE_NV = 0x871C,
GL_TEXTURE_DS_SIZE_NV = 0x871D,
GL_TEXTURE_DT_SIZE_NV = 0x871E,
GL_TEXTURE_MAG_SIZE_NV = 0x871F,
//GL_UNSIGNED_INT_S8_S8_8_8_NV = 0x86DA,
//GL_UNSIGNED_INT_8_8_S8_S8_REV_NV = 0x86DB,
//GL_DSDT_MAG_INTENSITY_NV = 0x86DC,
GL_DOT_PRODUCT_TEXTURE_3D_NV = 0x86EF,
//GL_HILO_NV = 0x86F4,
//GL_DSDT_NV = 0x86F5,
//GL_DSDT_MAG_NV = 0x86F6,
//GL_DSDT_MAG_VIB_NV = 0x86F7,
//GL_HILO16_NV = 0x86F8,
//GL_SIGNED_HILO_NV = 0x86F9,
//GL_SIGNED_HILO16_NV = 0x86FA,
//GL_SIGNED_RGBA_NV = 0x86FB,
//GL_SIGNED_RGBA8_NV = 0x86FC,
//GL_SIGNED_RGB_NV = 0x86FE,
//GL_SIGNED_RGB8_NV = 0x86FF,
//GL_SIGNED_LUMINANCE_NV = 0x8701,
//GL_SIGNED_LUMINANCE8_NV = 0x8702,
//GL_SIGNED_LUMINANCE_ALPHA_NV = 0x8703,
//GL_SIGNED_LUMINANCE8_ALPHA8_NV = 0x8704,
//GL_SIGNED_ALPHA_NV = 0x8705,
//GL_SIGNED_ALPHA8_NV = 0x8706,
//GL_SIGNED_INTENSITY_NV = 0x8707,
//GL_SIGNED_INTENSITY8_NV = 0x8708,
//GL_DSDT8_NV = 0x8709,
//GL_DSDT8_MAG8_NV = 0x870A,
//GL_DSDT8_MAG8_INTENSITY8_NV = 0x870B,
//GL_SIGNED_RGB_UNSIGNED_ALPHA_NV = 0x870C,
//GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV = 0x870D,
GL_OFFSET_PROJECTIVE_TEXTURE_2D_NV = 0x8850,
GL_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV = 0x8851,
GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV = 0x8852,
GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV = 0x8853,
GL_OFFSET_HILO_TEXTURE_2D_NV = 0x8854,
GL_OFFSET_HILO_TEXTURE_RECTANGLE_NV = 0x8855,
GL_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV = 0x8856,
GL_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV = 0x8857,
GL_DEPENDENT_HILO_TEXTURE_2D_NV = 0x8858,
GL_DEPENDENT_RGB_TEXTURE_3D_NV = 0x8859,
GL_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV = 0x885A,
GL_DOT_PRODUCT_PASS_THROUGH_NV = 0x885B,
GL_DOT_PRODUCT_TEXTURE_1D_NV = 0x885C,
GL_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV = 0x885D,
GL_HILO8_NV = 0x885E,
GL_SIGNED_HILO8_NV = 0x885F,
GL_FORCE_BLUE_TO_ONE_NV = 0x8860,
GL_BACK_PRIMARY_COLOR_NV = 0x8C77,
GL_BACK_SECONDARY_COLOR_NV = 0x8C78,
GL_TEXTURE_COORD_NV = 0x8C79,
GL_CLIP_DISTANCE_NV = 0x8C7A,
GL_VERTEX_ID_NV = 0x8C7B,
GL_PRIMITIVE_ID_NV = 0x8C7C,
GL_GENERIC_ATTRIB_NV = 0x8C7D,
GL_TRANSFORM_FEEDBACK_ATTRIBS_NV = 0x8C7E,
GL_TRANSFORM_FEEDBACK_BUFFER_MODE_NV = 0x8C7F,
GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_NV = 0x8C80,
GL_ACTIVE_VARYINGS_NV = 0x8C81,
GL_ACTIVE_VARYING_MAX_LENGTH_NV = 0x8C82,
GL_TRANSFORM_FEEDBACK_VARYINGS_NV = 0x8C83,
GL_TRANSFORM_FEEDBACK_BUFFER_START_NV = 0x8C84,
GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_NV = 0x8C85,
GL_TRANSFORM_FEEDBACK_RECORD_NV = 0x8C86,
GL_PRIMITIVES_GENERATED_NV = 0x8C87,
GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_NV = 0x8C88,
GL_RASTERIZER_DISCARD_NV = 0x8C89,
GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_NV = 0x8C8A,
GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_NV = 0x8C8B,
GL_INTERLEAVED_ATTRIBS_NV = 0x8C8C,
GL_SEPARATE_ATTRIBS_NV = 0x8C8D,
GL_TRANSFORM_FEEDBACK_BUFFER_NV = 0x8C8E,
GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_NV = 0x8C8F,
GL_TRANSFORM_FEEDBACK_NV = 0x8E22,
GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED_NV = 0x8E23,
GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE_NV = 0x8E24,
GL_TRANSFORM_FEEDBACK_BINDING_NV = 0x8E25,
GL_SURFACE_STATE_NV = 0x86EB,
GL_SURFACE_REGISTERED_NV = 0x86FD,
GL_SURFACE_MAPPED_NV = 0x8700,
GL_WRITE_DISCARD_NV = 0x88BE,
GL_VERTEX_ARRAY_RANGE_NV = 0x851D,
GL_VERTEX_ARRAY_RANGE_LENGTH_NV = 0x851E,
GL_VERTEX_ARRAY_RANGE_VALID_NV = 0x851F,
GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV = 0x8520,
GL_VERTEX_ARRAY_RANGE_POINTER_NV = 0x8521,
GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV = 0x8533,
//GL_INT64_NV = 0x140E,
//GL_UNSIGNED_INT64_NV = 0x140F,
GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV = 0x8F1E,
GL_ELEMENT_ARRAY_UNIFIED_NV = 0x8F1F,
GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV = 0x8F20,
GL_VERTEX_ARRAY_ADDRESS_NV = 0x8F21,
GL_NORMAL_ARRAY_ADDRESS_NV = 0x8F22,
GL_COLOR_ARRAY_ADDRESS_NV = 0x8F23,
GL_INDEX_ARRAY_ADDRESS_NV = 0x8F24,
GL_TEXTURE_COORD_ARRAY_ADDRESS_NV = 0x8F25,
GL_EDGE_FLAG_ARRAY_ADDRESS_NV = 0x8F26,
GL_SECONDARY_COLOR_ARRAY_ADDRESS_NV = 0x8F27,
GL_FOG_COORD_ARRAY_ADDRESS_NV = 0x8F28,
GL_ELEMENT_ARRAY_ADDRESS_NV = 0x8F29,
GL_VERTEX_ATTRIB_ARRAY_LENGTH_NV = 0x8F2A,
GL_VERTEX_ARRAY_LENGTH_NV = 0x8F2B,
GL_NORMAL_ARRAY_LENGTH_NV = 0x8F2C,
GL_COLOR_ARRAY_LENGTH_NV = 0x8F2D,
GL_INDEX_ARRAY_LENGTH_NV = 0x8F2E,
GL_TEXTURE_COORD_ARRAY_LENGTH_NV = 0x8F2F,
GL_EDGE_FLAG_ARRAY_LENGTH_NV = 0x8F30,
GL_SECONDARY_COLOR_ARRAY_LENGTH_NV = 0x8F31,
GL_FOG_COORD_ARRAY_LENGTH_NV = 0x8F32,
GL_ELEMENT_ARRAY_LENGTH_NV = 0x8F33,
GL_DRAW_INDIRECT_UNIFIED_NV = 0x8F40,
GL_DRAW_INDIRECT_ADDRESS_NV = 0x8F41,
GL_DRAW_INDIRECT_LENGTH_NV = 0x8F42,
GL_VERTEX_PROGRAM_NV = 0x8620,
GL_VERTEX_STATE_PROGRAM_NV = 0x8621,
GL_ATTRIB_ARRAY_SIZE_NV = 0x8623,
GL_ATTRIB_ARRAY_STRIDE_NV = 0x8624,
GL_ATTRIB_ARRAY_TYPE_NV = 0x8625,
GL_CURRENT_ATTRIB_NV = 0x8626,
GL_PROGRAM_LENGTH_NV = 0x8627,
GL_PROGRAM_STRING_NV = 0x8628,
GL_MODELVIEW_PROJECTION_NV = 0x8629,
GL_IDENTITY_NV = 0x862A,
GL_INVERSE_NV = 0x862B,
GL_TRANSPOSE_NV = 0x862C,
GL_INVERSE_TRANSPOSE_NV = 0x862D,
GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV = 0x862E,
GL_MAX_TRACK_MATRICES_NV = 0x862F,
GL_MATRIX0_NV = 0x8630,
GL_MATRIX1_NV = 0x8631,
GL_MATRIX2_NV = 0x8632,
GL_MATRIX3_NV = 0x8633,
GL_MATRIX4_NV = 0x8634,
GL_MATRIX5_NV = 0x8635,
GL_MATRIX6_NV = 0x8636,
GL_MATRIX7_NV = 0x8637,
GL_CURRENT_MATRIX_STACK_DEPTH_NV = 0x8640,
GL_CURRENT_MATRIX_NV = 0x8641,
GL_VERTEX_PROGRAM_POINT_SIZE_NV = 0x8642,
GL_VERTEX_PROGRAM_TWO_SIDE_NV = 0x8643,
GL_PROGRAM_PARAMETER_NV = 0x8644,
GL_ATTRIB_ARRAY_POINTER_NV = 0x8645,
GL_PROGRAM_TARGET_NV = 0x8646,
GL_PROGRAM_RESIDENT_NV = 0x8647,
GL_TRACK_MATRIX_NV = 0x8648,
GL_TRACK_MATRIX_TRANSFORM_NV = 0x8649,
GL_VERTEX_PROGRAM_BINDING_NV = 0x864A,
GL_PROGRAM_ERROR_POSITION_NV = 0x864B,
GL_VERTEX_ATTRIB_ARRAY0_NV = 0x8650,
GL_VERTEX_ATTRIB_ARRAY1_NV = 0x8651,
GL_VERTEX_ATTRIB_ARRAY2_NV = 0x8652,
GL_VERTEX_ATTRIB_ARRAY3_NV = 0x8653,
GL_VERTEX_ATTRIB_ARRAY4_NV = 0x8654,
GL_VERTEX_ATTRIB_ARRAY5_NV = 0x8655,
GL_VERTEX_ATTRIB_ARRAY6_NV = 0x8656,
GL_VERTEX_ATTRIB_ARRAY7_NV = 0x8657,
GL_VERTEX_ATTRIB_ARRAY8_NV = 0x8658,
GL_VERTEX_ATTRIB_ARRAY9_NV = 0x8659,
GL_VERTEX_ATTRIB_ARRAY10_NV = 0x865A,
GL_VERTEX_ATTRIB_ARRAY11_NV = 0x865B,
GL_VERTEX_ATTRIB_ARRAY12_NV = 0x865C,
GL_VERTEX_ATTRIB_ARRAY13_NV = 0x865D,
GL_VERTEX_ATTRIB_ARRAY14_NV = 0x865E,
GL_VERTEX_ATTRIB_ARRAY15_NV = 0x865F,
GL_MAP1_VERTEX_ATTRIB0_4_NV = 0x8660,
GL_MAP1_VERTEX_ATTRIB1_4_NV = 0x8661,
GL_MAP1_VERTEX_ATTRIB2_4_NV = 0x8662,
GL_MAP1_VERTEX_ATTRIB3_4_NV = 0x8663,
GL_MAP1_VERTEX_ATTRIB4_4_NV = 0x8664,
GL_MAP1_VERTEX_ATTRIB5_4_NV = 0x8665,
GL_MAP1_VERTEX_ATTRIB6_4_NV = 0x8666,
GL_MAP1_VERTEX_ATTRIB7_4_NV = 0x8667,
GL_MAP1_VERTEX_ATTRIB8_4_NV = 0x8668,
GL_MAP1_VERTEX_ATTRIB9_4_NV = 0x8669,
GL_MAP1_VERTEX_ATTRIB10_4_NV = 0x866A,
GL_MAP1_VERTEX_ATTRIB11_4_NV = 0x866B,
GL_MAP1_VERTEX_ATTRIB12_4_NV = 0x866C,
GL_MAP1_VERTEX_ATTRIB13_4_NV = 0x866D,
GL_MAP1_VERTEX_ATTRIB14_4_NV = 0x866E,
GL_MAP1_VERTEX_ATTRIB15_4_NV = 0x866F,
GL_MAP2_VERTEX_ATTRIB0_4_NV = 0x8670,
GL_MAP2_VERTEX_ATTRIB1_4_NV = 0x8671,
GL_MAP2_VERTEX_ATTRIB2_4_NV = 0x8672,
GL_MAP2_VERTEX_ATTRIB3_4_NV = 0x8673,
GL_MAP2_VERTEX_ATTRIB4_4_NV = 0x8674,
GL_MAP2_VERTEX_ATTRIB5_4_NV = 0x8675,
GL_MAP2_VERTEX_ATTRIB6_4_NV = 0x8676,
GL_MAP2_VERTEX_ATTRIB7_4_NV = 0x8677,
GL_MAP2_VERTEX_ATTRIB8_4_NV = 0x8678,
GL_MAP2_VERTEX_ATTRIB9_4_NV = 0x8679,
GL_MAP2_VERTEX_ATTRIB10_4_NV = 0x867A,
GL_MAP2_VERTEX_ATTRIB11_4_NV = 0x867B,
GL_MAP2_VERTEX_ATTRIB12_4_NV = 0x867C,
GL_MAP2_VERTEX_ATTRIB13_4_NV = 0x867D,
GL_MAP2_VERTEX_ATTRIB14_4_NV = 0x867E,
GL_MAP2_VERTEX_ATTRIB15_4_NV = 0x867F,
//GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV = 0x88F4,
//GL_MAX_PROGRAM_CALL_DEPTH_NV = 0x88F5,
GL_VERTEX_ATTRIB_ARRAY_INTEGER_NV = 0x88FD,
};

extern void ( * glAccum) (GLenum op, GLfloat value);
extern void ( * glAlphaFunc) (GLenum func, GLclampf ref);
extern GLboolean ( * glAreTexturesResident) (GLsizei n, const GLuint *textures, GLboolean *residences);
extern void ( * glArrayElement) (GLint i);
extern void ( * glBegin) (GLenum mode);
extern void ( * glBindTexture) (GLenum target, GLuint texture);
extern void ( * glBitmap) (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
extern void ( * glBlendFunc) (GLenum sfactor, GLenum dfactor);
extern void ( * glCallList) (GLuint list);
extern void ( * glCallLists) (GLsizei n, GLenum type, const GLvoid *lists);
extern void ( * glClear) (GLbitfield mask);
extern void ( * glClearAccum) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void ( * glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void ( * glClearDepth) (GLclampd depth);
extern void ( * glClearIndex) (GLfloat c);
extern void ( * glClearStencil) (GLint s);
extern void ( * glClipPlane) (GLenum plane, const GLdouble *equation);
extern void ( * glColor3b) (GLbyte red, GLbyte green, GLbyte blue);
extern void ( * glColor3bv) (const GLbyte *v);
extern void ( * glColor3d) (GLdouble red, GLdouble green, GLdouble blue);
extern void ( * glColor3dv) (const GLdouble *v);
extern void ( * glColor3f) (GLfloat red, GLfloat green, GLfloat blue);
extern void ( * glColor3fv) (const GLfloat *v);
extern void ( * glColor3i) (GLint red, GLint green, GLint blue);
extern void ( * glColor3iv) (const GLint *v);
extern void ( * glColor3s) (GLshort red, GLshort green, GLshort blue);
extern void ( * glColor3sv) (const GLshort *v);
extern void ( * glColor3ub) (GLubyte red, GLubyte green, GLubyte blue);
extern void ( * glColor3ubv) (const GLubyte *v);
extern void ( * glColor3ui) (GLuint red, GLuint green, GLuint blue);
extern void ( * glColor3uiv) (const GLuint *v);
extern void ( * glColor3us) (GLushort red, GLushort green, GLushort blue);
extern void ( * glColor3usv) (const GLushort *v);
extern void ( * glColor4b) (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
extern void ( * glColor4bv) (const GLbyte *v);
extern void ( * glColor4d) (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
extern void ( * glColor4dv) (const GLdouble *v);
extern void ( * glColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void ( * glColor4fv) (const GLfloat *v);
extern void ( * glColor4i) (GLint red, GLint green, GLint blue, GLint alpha);
extern void ( * glColor4iv) (const GLint *v);
extern void ( * glColor4s) (GLshort red, GLshort green, GLshort blue, GLshort alpha);
extern void ( * glColor4sv) (const GLshort *v);
extern void ( * glColor4ub) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
extern void ( * glColor4ubv) (const GLubyte *v);
extern void ( * glColor4ui) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
extern void ( * glColor4uiv) (const GLuint *v);
extern void ( * glColor4us) (GLushort red, GLushort green, GLushort blue, GLushort alpha);
extern void ( * glColor4usv) (const GLushort *v);
extern void ( * glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern void ( * glColorMaterial) (GLenum face, GLenum mode);
extern void ( * glColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void ( * glCopyPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
extern void ( * glCullFace) (GLenum mode);
extern void ( * glDeleteLists) (GLuint list, GLsizei range);
extern void ( * glDeleteTextures) (GLsizei n, const GLuint *textures);
extern void ( * glDepthFunc) (GLenum func);
extern void ( * glDepthMask) (GLboolean flag);
extern void ( * glDepthRange) (GLclampd zNear, GLclampd zFar);
extern void ( * glDisable) (GLenum cap);
extern void ( * glDisableClientState) (GLenum array);
extern void ( * glDrawArrays) (GLenum mode, GLint first, GLsizei count);
extern void ( * glDrawBuffer) (GLenum mode);
extern void ( * glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
extern void ( * glDrawPixels) (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern void ( * glEdgeFlag) (GLboolean flag);
extern void ( * glEdgeFlagPointer) (GLsizei stride, const GLvoid *pointer);
extern void ( * glEdgeFlagv) (const GLboolean *flag);
extern void ( * glEnable) (GLenum cap);
extern void ( * glEnableClientState) (GLenum array);
extern void ( * glEnd) (void);
extern void ( * glEndList) (void);
extern void ( * glEvalCoord1d) (GLdouble u);
extern void ( * glEvalCoord1dv) (const GLdouble *u);
extern void ( * glEvalCoord1f) (GLfloat u);
extern void ( * glEvalCoord1fv) (const GLfloat *u);
extern void ( * glEvalCoord2d) (GLdouble u, GLdouble v);
extern void ( * glEvalCoord2dv) (const GLdouble *u);
extern void ( * glEvalCoord2f) (GLfloat u, GLfloat v);
extern void ( * glEvalCoord2fv) (const GLfloat *u);
extern void ( * glEvalMesh1) (GLenum mode, GLint i1, GLint i2);
extern void ( * glEvalMesh2) (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
extern void ( * glEvalPoint1) (GLint i);
extern void ( * glEvalPoint2) (GLint i, GLint j);
extern void ( * glFeedbackBuffer) (GLsizei size, GLenum type, GLfloat *buffer);
extern void ( * glFinish) (void);
extern void ( * glFlush) (void);
extern void ( * glFogf) (GLenum pname, GLfloat param);
extern void ( * glFogfv) (GLenum pname, const GLfloat *params);
extern void ( * glFogi) (GLenum pname, GLint param);
extern void ( * glFogiv) (GLenum pname, const GLint *params);
extern void ( * glFrontFace) (GLenum mode);
extern void ( * glFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern GLuint ( * glGenLists) (GLsizei range);
extern void ( * glGenTextures) (GLsizei n, GLuint *textures);
extern void ( * glGetBooleanv) (GLenum pname, GLboolean *params);
extern void ( * glGetClipPlane) (GLenum plane, GLdouble *equation);
extern void ( * glGetDoublev) (GLenum pname, GLdouble *params);
extern GLenum ( * glGetError) (void);
extern void ( * glGetFloatv) (GLenum pname, GLfloat *params);
extern void ( * glGetIntegerv) (GLenum pname, GLint *params);
extern void ( * glGetLightfv) (GLenum light, GLenum pname, GLfloat *params);
extern void ( * glGetLightiv) (GLenum light, GLenum pname, GLint *params);
extern void ( * glGetMapdv) (GLenum target, GLenum query, GLdouble *v);
extern void ( * glGetMapfv) (GLenum target, GLenum query, GLfloat *v);
extern void ( * glGetMapiv) (GLenum target, GLenum query, GLint *v);
extern void ( * glGetMaterialfv) (GLenum face, GLenum pname, GLfloat *params);
extern void ( * glGetMaterialiv) (GLenum face, GLenum pname, GLint *params);
extern void ( * glGetPixelMapfv) (GLenum map, GLfloat *values);
extern void ( * glGetPixelMapuiv) (GLenum map, GLuint *values);
extern void ( * glGetPixelMapusv) (GLenum map, GLushort *values);
extern void ( * glGetPointerv) (GLenum pname, GLvoid* *params);
extern void ( * glGetPolygonStipple) (GLubyte *mask);
extern GLubyte* ( * glGetString) (GLenum s);
extern void ( * glGetTexEnvfv) (GLenum target, GLenum pname, GLfloat *params);
extern void ( * glGetTexEnviv) (GLenum target, GLenum pname, GLint *params);
extern void ( * glGetTexGendv) (GLenum coord, GLenum pname, GLdouble *params);
extern void ( * glGetTexGenfv) (GLenum coord, GLenum pname, GLfloat *params);
extern void ( * glGetTexGeniv) (GLenum coord, GLenum pname, GLint *params);
extern void ( * glGetTexImage) (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
extern void ( * glGetTexLevelParameterfv) (GLenum target, GLint level, GLenum pname, GLfloat *params);
extern void ( * glGetTexLevelParameteriv) (GLenum target, GLint level, GLenum pname, GLint *params);
extern void ( * glGetTexParameterfv) (GLenum target, GLenum pname, GLfloat *params);
extern void ( * glGetTexParameteriv) (GLenum target, GLenum pname, GLint *params);
extern void ( * glHint) (GLenum target, GLenum mode);
extern void ( * glIndexMask) (GLuint mask);
extern void ( * glIndexPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void ( * glIndexd) (GLdouble c);
extern void ( * glIndexdv) (const GLdouble *c);
extern void ( * glIndexf) (GLfloat c);
extern void ( * glIndexfv) (const GLfloat *c);
extern void ( * glIndexi) (GLint c);
extern void ( * glIndexiv) (const GLint *c);
extern void ( * glIndexs) (GLshort c);
extern void ( * glIndexsv) (const GLshort *c);
extern void ( * glIndexub) (GLubyte c);
extern void ( * glIndexubv) (const GLubyte *c);
extern void ( * glInitNames) (void);
extern void ( * glInterleavedArrays) (GLenum format, GLsizei stride, const GLvoid *pointer);
extern GLboolean ( * glIsEnabled) (GLenum cap);
extern GLboolean ( * glIsList) (GLuint list);
extern GLboolean ( * glIsTexture) (GLuint texture);
extern void ( * glLightModelf) (GLenum pname, GLfloat param);
extern void ( * glLightModelfv) (GLenum pname, const GLfloat *params);
extern void ( * glLightModeli) (GLenum pname, GLint param);
extern void ( * glLightModeliv) (GLenum pname, const GLint *params);
extern void ( * glLightf) (GLenum light, GLenum pname, GLfloat param);
extern void ( * glLightfv) (GLenum light, GLenum pname, const GLfloat *params);
extern void ( * glLighti) (GLenum light, GLenum pname, GLint param);
extern void ( * glLightiv) (GLenum light, GLenum pname, const GLint *params);
extern void ( * glLineStipple) (GLint factor, GLushort pattern);
extern void ( * glLineWidth) (GLfloat width);
extern void ( * glListBase) (GLuint base);
extern void ( * glLoadIdentity) (void);
extern void ( * glLoadMatrixd) (const GLdouble *m);
extern void ( * glLoadMatrixf) (const GLfloat *m);
extern void ( * glLoadName) (GLuint name);
extern void ( * glLogicOp) (GLenum opcode);
extern void ( * glMap1d) (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
extern void ( * glMap1f) (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
extern void ( * glMap2d) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
extern void ( * glMap2f) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
extern void ( * glMapGrid1d) (GLint un, GLdouble u1, GLdouble u2);
extern void ( * glMapGrid1f) (GLint un, GLfloat u1, GLfloat u2);
extern void ( * glMapGrid2d) (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
extern void ( * glMapGrid2f) (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
extern void ( * glMaterialf) (GLenum face, GLenum pname, GLfloat param);
extern void ( * glMaterialfv) (GLenum face, GLenum pname, const GLfloat *params);
extern void ( * glMateriali) (GLenum face, GLenum pname, GLint param);
extern void ( * glMaterialiv) (GLenum face, GLenum pname, const GLint *params);
extern void ( * glMatrixMode) (GLenum mode);
extern void ( * glMultMatrixd) (const GLdouble *m);
extern void ( * glMultMatrixf) (const GLfloat *m);
extern void ( * glNewList) (GLuint list, GLenum mode);
extern void ( * glNormal3b) (GLbyte nx, GLbyte ny, GLbyte nz);
extern void ( * glNormal3bv) (const GLbyte *v);
extern void ( * glNormal3d) (GLdouble nx, GLdouble ny, GLdouble nz);
extern void ( * glNormal3dv) (const GLdouble *v);
extern void ( * glNormal3f) (GLfloat nx, GLfloat ny, GLfloat nz);
extern void ( * glNormal3fv) (const GLfloat *v);
extern void ( * glNormal3i) (GLint nx, GLint ny, GLint nz);
extern void ( * glNormal3iv) (const GLint *v);
extern void ( * glNormal3s) (GLshort nx, GLshort ny, GLshort nz);
extern void ( * glNormal3sv) (const GLshort *v);
extern void ( * glNormalPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void ( * glOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern void ( * glPassThrough) (GLfloat token);
extern void ( * glPixelMapfv) (GLenum map, GLsizei mapsize, const GLfloat *values);
extern void ( * glPixelMapuiv) (GLenum map, GLsizei mapsize, const GLuint *values);
extern void ( * glPixelMapusv) (GLenum map, GLsizei mapsize, const GLushort *values);
extern void ( * glPixelStoref) (GLenum pname, GLfloat param);
extern void ( * glPixelStorei) (GLenum pname, GLint param);
extern void ( * glPixelTransferf) (GLenum pname, GLfloat param);
extern void ( * glPixelTransferi) (GLenum pname, GLint param);
extern void ( * glPixelZoom) (GLfloat xfactor, GLfloat yfactor);
extern void ( * glPointSize) (GLfloat size);
extern void ( * glPolygonMode) (GLenum face, GLenum mode);
extern void ( * glPolygonOffset) (GLfloat factor, GLfloat units);
extern void ( * glPolygonStipple) (const GLubyte *mask);
extern void ( * glPopAttrib) (void);
extern void ( * glPopClientAttrib) (void);
extern void ( * glPopMatrix) (void);
extern void ( * glPopName) (void);
extern void ( * glPrioritizeTextures) (GLsizei n, const GLuint *textures, const GLclampf *priorities);
extern void ( * glPushAttrib) (GLbitfield mask);
extern void ( * glPushClientAttrib) (GLbitfield mask);
extern void ( * glPushMatrix) (void);
extern void ( * glPushName) (GLuint name);
extern void ( * glRasterPos2d) (GLdouble x, GLdouble y);
extern void ( * glRasterPos2dv) (const GLdouble *v);
extern void ( * glRasterPos2f) (GLfloat x, GLfloat y);
extern void ( * glRasterPos2fv) (const GLfloat *v);
extern void ( * glRasterPos2i) (GLint x, GLint y);
extern void ( * glRasterPos2iv) (const GLint *v);
extern void ( * glRasterPos2s) (GLshort x, GLshort y);
extern void ( * glRasterPos2sv) (const GLshort *v);
extern void ( * glRasterPos3d) (GLdouble x, GLdouble y, GLdouble z);
extern void ( * glRasterPos3dv) (const GLdouble *v);
extern void ( * glRasterPos3f) (GLfloat x, GLfloat y, GLfloat z);
extern void ( * glRasterPos3fv) (const GLfloat *v);
extern void ( * glRasterPos3i) (GLint x, GLint y, GLint z);
extern void ( * glRasterPos3iv) (const GLint *v);
extern void ( * glRasterPos3s) (GLshort x, GLshort y, GLshort z);
extern void ( * glRasterPos3sv) (const GLshort *v);
extern void ( * glRasterPos4d) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void ( * glRasterPos4dv) (const GLdouble *v);
extern void ( * glRasterPos4f) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void ( * glRasterPos4fv) (const GLfloat *v);
extern void ( * glRasterPos4i) (GLint x, GLint y, GLint z, GLint w);
extern void ( * glRasterPos4iv) (const GLint *v);
extern void ( * glRasterPos4s) (GLshort x, GLshort y, GLshort z, GLshort w);
extern void ( * glRasterPos4sv) (const GLshort *v);
extern void ( * glReadBuffer) (GLenum mode);
extern void ( * glReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
extern void ( * glRectd) (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
extern void ( * glRectdv) (const GLdouble *v1, const GLdouble *v2);
extern void ( * glRectf) (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
extern void ( * glRectfv) (const GLfloat *v1, const GLfloat *v2);
extern void ( * glRecti) (GLint x1, GLint y1, GLint x2, GLint y2);
extern void ( * glRectiv) (const GLint *v1, const GLint *v2);
extern void ( * glRects) (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
extern void ( * glRectsv) (const GLshort *v1, const GLshort *v2);
extern GLint ( * glRenderMode) (GLenum mode);
extern void ( * glRotated) (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
extern void ( * glRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern void ( * glScaled) (GLdouble x, GLdouble y, GLdouble z);
extern void ( * glScalef) (GLfloat x, GLfloat y, GLfloat z);
extern void ( * glScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
extern void ( * glSelectBuffer) (GLsizei size, GLuint *buffer);
extern void ( * glShadeModel) (GLenum mode);
extern void ( * glStencilFunc) (GLenum func, GLint ref, GLuint mask);
extern void ( * glStencilMask) (GLuint mask);
extern void ( * glStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
extern void ( * glTexCoord1d) (GLdouble s);
extern void ( * glTexCoord1dv) (const GLdouble *v);
extern void ( * glTexCoord1f) (GLfloat s);
extern void ( * glTexCoord1fv) (const GLfloat *v);
extern void ( * glTexCoord1i) (GLint s);
extern void ( * glTexCoord1iv) (const GLint *v);
extern void ( * glTexCoord1s) (GLshort s);
extern void ( * glTexCoord1sv) (const GLshort *v);
extern void ( * glTexCoord2d) (GLdouble s, GLdouble t);
extern void ( * glTexCoord2dv) (const GLdouble *v);
extern void ( * glTexCoord2f) (GLfloat s, GLfloat t);
extern void ( * glTexCoord2fv) (const GLfloat *v);
extern void ( * glTexCoord2i) (GLint s, GLint t);
extern void ( * glTexCoord2iv) (const GLint *v);
extern void ( * glTexCoord2s) (GLshort s, GLshort t);
extern void ( * glTexCoord2sv) (const GLshort *v);
extern void ( * glTexCoord3d) (GLdouble s, GLdouble t, GLdouble r);
extern void ( * glTexCoord3dv) (const GLdouble *v);
extern void ( * glTexCoord3f) (GLfloat s, GLfloat t, GLfloat r);
extern void ( * glTexCoord3fv) (const GLfloat *v);
extern void ( * glTexCoord3i) (GLint s, GLint t, GLint r);
extern void ( * glTexCoord3iv) (const GLint *v);
extern void ( * glTexCoord3s) (GLshort s, GLshort t, GLshort r);
extern void ( * glTexCoord3sv) (const GLshort *v);
extern void ( * glTexCoord4d) (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern void ( * glTexCoord4dv) (const GLdouble *v);
extern void ( * glTexCoord4f) (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void ( * glTexCoord4fv) (const GLfloat *v);
extern void ( * glTexCoord4i) (GLint s, GLint t, GLint r, GLint q);
extern void ( * glTexCoord4iv) (const GLint *v);
extern void ( * glTexCoord4s) (GLshort s, GLshort t, GLshort r, GLshort q);
extern void ( * glTexCoord4sv) (const GLshort *v);
extern void ( * glTexCoordPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void ( * glTexEnvf) (GLenum target, GLenum pname, GLfloat param);
extern void ( * glTexEnvfv) (GLenum target, GLenum pname, const GLfloat *params);
extern void ( * glTexEnvi) (GLenum target, GLenum pname, GLint param);
extern void ( * glTexEnviv) (GLenum target, GLenum pname, const GLint *params);
extern void ( * glTexGend) (GLenum coord, GLenum pname, GLdouble param);
extern void ( * glTexGendv) (GLenum coord, GLenum pname, const GLdouble *params);
extern void ( * glTexGenf) (GLenum coord, GLenum pname, GLfloat param);
extern void ( * glTexGenfv) (GLenum coord, GLenum pname, const GLfloat *params);
extern void ( * glTexGeni) (GLenum coord, GLenum pname, GLint param);
extern void ( * glTexGeniv) (GLenum coord, GLenum pname, const GLint *params);
extern void ( * glTexParameterf) (GLenum target, GLenum pname, GLfloat param);
extern void ( * glTexParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
extern void ( * glTexParameteri) (GLenum target, GLenum pname, GLint param);
extern void ( * glTexParameteriv) (GLenum target, GLenum pname, const GLint *params);
extern void ( * glTranslated) (GLdouble x, GLdouble y, GLdouble z);
extern void ( * glTranslatef) (GLfloat x, GLfloat y, GLfloat z);
extern void ( * glVertex2d) (GLdouble x, GLdouble y);
extern void ( * glVertex2dv) (const GLdouble *v);
extern void ( * glVertex2f) (GLfloat x, GLfloat y);
extern void ( * glVertex2fv) (const GLfloat *v);
extern void ( * glVertex2i) (GLint x, GLint y);
extern void ( * glVertex2iv) (const GLint *v);
extern void ( * glVertex2s) (GLshort x, GLshort y);
extern void ( * glVertex2sv) (const GLshort *v);
extern void ( * glVertex3d) (GLdouble x, GLdouble y, GLdouble z);
extern void ( * glVertex3dv) (const GLdouble *v);
extern void ( * glVertex3f) (GLfloat x, GLfloat y, GLfloat z);
extern void ( * glVertex3fv) (const GLfloat *v);
extern void ( * glVertex3i) (GLint x, GLint y, GLint z);
extern void ( * glVertex3iv) (const GLint *v);
extern void ( * glVertex3s) (GLshort x, GLshort y, GLshort z);
extern void ( * glVertex3sv) (const GLshort *v);
extern void ( * glVertex4d) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void ( * glVertex4dv) (const GLdouble *v);
extern void ( * glVertex4f) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void ( * glVertex4fv) (const GLfloat *v);
extern void ( * glVertex4i) (GLint x, GLint y, GLint z, GLint w);
extern void ( * glVertex4iv) (const GLint *v);
extern void ( * glVertex4s) (GLshort x, GLshort y, GLshort z, GLshort w);
extern void ( * glVertex4sv) (const GLshort *v);
extern void ( * glVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void ( * glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
extern void ( * glDrawRangeElements) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
extern void ( * glActiveTexture) (GLenum texture);
extern void ( * glClientActiveTexture) (GLenum texture);
extern void ( * glGetCompressedTexImage) (GLenum target, GLint lod, GLvoid *img);
extern void ( * glLoadTransposeMatrixd) (const GLdouble m[16]);
extern void ( * glLoadTransposeMatrixf) (const GLfloat m[16]);
extern void ( * glMultTransposeMatrixd) (const GLdouble m[16]);
extern void ( * glMultTransposeMatrixf) (const GLfloat m[16]);
extern void ( * glMultiTexCoord1d) (GLenum target, GLdouble s);
extern void ( * glMultiTexCoord1dv) (GLenum target, const GLdouble *v);
extern void ( * glMultiTexCoord1f) (GLenum target, GLfloat s);
extern void ( * glMultiTexCoord1fv) (GLenum target, const GLfloat *v);
extern void ( * glMultiTexCoord1i) (GLenum target, GLint s);
extern void ( * glMultiTexCoord1iv) (GLenum target, const GLint *v);
extern void ( * glMultiTexCoord1s) (GLenum target, GLshort s);
extern void ( * glMultiTexCoord1sv) (GLenum target, const GLshort *v);
extern void ( * glMultiTexCoord2d) (GLenum target, GLdouble s, GLdouble t);
extern void ( * glMultiTexCoord2dv) (GLenum target, const GLdouble *v);
extern void ( * glMultiTexCoord2f) (GLenum target, GLfloat s, GLfloat t);
extern void ( * glMultiTexCoord2fv) (GLenum target, const GLfloat *v);
extern void ( * glMultiTexCoord2i) (GLenum target, GLint s, GLint t);
extern void ( * glMultiTexCoord2iv) (GLenum target, const GLint *v);
extern void ( * glMultiTexCoord2s) (GLenum target, GLshort s, GLshort t);
extern void ( * glMultiTexCoord2sv) (GLenum target, const GLshort *v);
extern void ( * glMultiTexCoord3d) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
extern void ( * glMultiTexCoord3dv) (GLenum target, const GLdouble *v);
extern void ( * glMultiTexCoord3f) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
extern void ( * glMultiTexCoord3fv) (GLenum target, const GLfloat *v);
extern void ( * glMultiTexCoord3i) (GLenum target, GLint s, GLint t, GLint r);
extern void ( * glMultiTexCoord3iv) (GLenum target, const GLint *v);
extern void ( * glMultiTexCoord3s) (GLenum target, GLshort s, GLshort t, GLshort r);
extern void ( * glMultiTexCoord3sv) (GLenum target, const GLshort *v);
extern void ( * glMultiTexCoord4d) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern void ( * glMultiTexCoord4dv) (GLenum target, const GLdouble *v);
extern void ( * glMultiTexCoord4f) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void ( * glMultiTexCoord4fv) (GLenum target, const GLfloat *v);
extern void ( * glMultiTexCoord4i) (GLenum target, GLint s, GLint t, GLint r, GLint q);
extern void ( * glMultiTexCoord4iv) (GLenum target, const GLint *v);
extern void ( * glMultiTexCoord4s) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
extern void ( * glMultiTexCoord4sv) (GLenum target, const GLshort *v);
extern void ( * glSampleCoverage) (GLclampf value, GLboolean invert);
extern void ( * glBlendColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void ( * glBlendEquation) (GLenum mode);
extern void ( * glBlendFuncSeparate) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
extern void ( * glFogCoordPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void ( * glFogCoordd) (GLdouble coord);
extern void ( * glFogCoorddv) (const GLdouble *coord);
extern void ( * glFogCoordf) (GLfloat coord);
extern void ( * glFogCoordfv) (const GLfloat *coord);
extern void ( * glMultiDrawArrays) (GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
extern void ( * glMultiDrawElements) (GLenum mode, GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount);
extern void ( * glPointParameterf) (GLenum pname, GLfloat param);
extern void ( * glPointParameterfv) (GLenum pname, const GLfloat *params);
extern void ( * glPointParameteri) (GLenum pname, GLint param);
extern void ( * glPointParameteriv) (GLenum pname, const GLint *params);
extern void ( * glSecondaryColor3b) (GLbyte red, GLbyte green, GLbyte blue);
extern void ( * glSecondaryColor3bv) (const GLbyte *v);
extern void ( * glSecondaryColor3d) (GLdouble red, GLdouble green, GLdouble blue);
extern void ( * glSecondaryColor3dv) (const GLdouble *v);
extern void ( * glSecondaryColor3f) (GLfloat red, GLfloat green, GLfloat blue);
extern void ( * glSecondaryColor3fv) (const GLfloat *v);
extern void ( * glSecondaryColor3i) (GLint red, GLint green, GLint blue);
extern void ( * glSecondaryColor3iv) (const GLint *v);
extern void ( * glSecondaryColor3s) (GLshort red, GLshort green, GLshort blue);
extern void ( * glSecondaryColor3sv) (const GLshort *v);
extern void ( * glSecondaryColor3ub) (GLubyte red, GLubyte green, GLubyte blue);
extern void ( * glSecondaryColor3ubv) (const GLubyte *v);
extern void ( * glSecondaryColor3ui) (GLuint red, GLuint green, GLuint blue);
extern void ( * glSecondaryColor3uiv) (const GLuint *v);
extern void ( * glSecondaryColor3us) (GLushort red, GLushort green, GLushort blue);
extern void ( * glSecondaryColor3usv) (const GLushort *v);
extern void ( * glSecondaryColorPointer) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
extern void ( * glWindowPos2d) (GLdouble x, GLdouble y);
extern void ( * glWindowPos2dv) (const GLdouble *p);
extern void ( * glWindowPos2f) (GLfloat x, GLfloat y);
extern void ( * glWindowPos2fv) (const GLfloat *p);
extern void ( * glWindowPos2i) (GLint x, GLint y);
extern void ( * glWindowPos2iv) (const GLint *p);
extern void ( * glWindowPos2s) (GLshort x, GLshort y);
extern void ( * glWindowPos2sv) (const GLshort *p);
extern void ( * glWindowPos3d) (GLdouble x, GLdouble y, GLdouble z);
extern void ( * glWindowPos3dv) (const GLdouble *p);
extern void ( * glWindowPos3f) (GLfloat x, GLfloat y, GLfloat z);
extern void ( * glWindowPos3fv) (const GLfloat *p);
extern void ( * glWindowPos3i) (GLint x, GLint y, GLint z);
extern void ( * glWindowPos3iv) (const GLint *p);
extern void ( * glWindowPos3s) (GLshort x, GLshort y, GLshort z);
extern void ( * glWindowPos3sv) (const GLshort *p);
extern void ( * glBeginQuery) (GLenum target, GLuint id);
extern void ( * glBindBuffer) (GLenum target, GLuint buffer);
extern void ( * glBufferData) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
extern void ( * glBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
extern void ( * glDeleteBuffers) (GLsizei n, const GLuint* buffers);
extern void ( * glDeleteQueries) (GLsizei n, const GLuint* ids);
extern void ( * glEndQuery) (GLenum target);
extern void ( * glGenBuffers) (GLsizei n, GLuint* buffers);
extern void ( * glGenQueries) (GLsizei n, GLuint* ids);
extern void ( * glGetBufferParameteriv) (GLenum target, GLenum pname, GLint* params);
extern void ( * glGetBufferPointerv) (GLenum target, GLenum pname, GLvoid** params);
extern void ( * glGetBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid* data);
extern void ( * glGetQueryObjectiv) (GLuint id, GLenum pname, GLint* params);
extern void ( * glGetQueryObjectuiv) (GLuint id, GLenum pname, GLuint* params);
extern void ( * glGetQueryiv) (GLenum target, GLenum pname, GLint* params);
extern GLboolean ( * glIsBuffer) (GLuint buffer);
extern GLboolean ( * glIsQuery) (GLuint id);
extern GLvoid* ( * glMapBuffer) (GLenum target, GLenum access);
extern GLboolean ( * glUnmapBuffer) (GLenum target);
extern void ( * glAttachShader) (GLuint program, GLuint shader);
extern void ( * glBindAttribLocation) (GLuint program, GLuint index, const GLchar* name);
extern void ( * glBlendEquationSeparate) (GLenum, GLenum);
extern void ( * glCompileShader) (GLuint shader);
extern GLuint ( * glCreateProgram) (void);
extern GLuint ( * glCreateShader) (GLenum type);
extern void ( * glDeleteProgram) (GLuint program);
extern void ( * glDeleteShader) (GLuint shader);
extern void ( * glDetachShader) (GLuint program, GLuint shader);
extern void ( * glDisableVertexAttribArray) (GLuint);
extern void ( * glDrawBuffers) (GLsizei n, const GLenum* bufs);
extern void ( * glEnableVertexAttribArray) (GLuint);
extern void ( * glGetActiveAttrib) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
extern void ( * glGetActiveUniform) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
extern void ( * glGetAttachedShaders) (GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
extern GLint ( * glGetAttribLocation) (GLuint program, const GLchar* name);
extern void ( * glGetProgramInfoLog) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
extern void ( * glGetProgramiv) (GLuint program, GLenum pname, GLint* param);
extern void ( * glGetShaderInfoLog) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
extern void ( * glGetShaderSource) (GLint obj, GLsizei maxLength, GLsizei* length, GLchar* source);
extern void ( * glGetShaderiv) (GLuint shader, GLenum pname, GLint* param);
extern GLint ( * glGetUniformLocation) (GLuint program, const GLchar* name);
extern void ( * glGetUniformfv) (GLuint program, GLint location, GLfloat* params);
extern void ( * glGetUniformiv) (GLuint program, GLint location, GLint* params);
extern void ( * glGetVertexAttribPointerv) (GLuint, GLenum, GLvoid*);
extern void ( * glGetVertexAttribdv) (GLuint, GLenum, GLdouble*);
extern void ( * glGetVertexAttribfv) (GLuint, GLenum, GLfloat*);
extern void ( * glGetVertexAttribiv) (GLuint, GLenum, GLint*);
extern GLboolean ( * glIsProgram) (GLuint program);
extern GLboolean ( * glIsShader) (GLuint shader);
extern void ( * glLinkProgram) (GLuint program);
extern void ( * glShaderSource) (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
extern void ( * glStencilFuncSeparate) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
extern void ( * glStencilMaskSeparate) (GLenum, GLuint);
extern void ( * glStencilOpSeparate) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern void ( * glUniform1f) (GLint location, GLfloat v0);
extern void ( * glUniform1fv) (GLint location, GLsizei count, const GLfloat* value);
extern void ( * glUniform1i) (GLint location, GLint v0);
extern void ( * glUniform1iv) (GLint location, GLsizei count, const GLint* value);
extern void ( * glUniform2f) (GLint location, GLfloat v0, GLfloat v1);
extern void ( * glUniform2fv) (GLint location, GLsizei count, const GLfloat* value);
extern void ( * glUniform2i) (GLint location, GLint v0, GLint v1);
extern void ( * glUniform2iv) (GLint location, GLsizei count, const GLint* value);
extern void ( * glUniform3f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void ( * glUniform3fv) (GLint location, GLsizei count, const GLfloat* value);
extern void ( * glUniform3i) (GLint location, GLint v0, GLint v1, GLint v2);
extern void ( * glUniform3iv) (GLint location, GLsizei count, const GLint* value);
extern void ( * glUniform4f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void ( * glUniform4fv) (GLint location, GLsizei count, const GLfloat* value);
extern void ( * glUniform4i) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void ( * glUniform4iv) (GLint location, GLsizei count, const GLint* value);
extern void ( * glUniformMatrix2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glUniformMatrix3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glUseProgram) (GLuint program);
extern void ( * glValidateProgram) (GLuint program);
extern void ( * glVertexAttrib1d) (GLuint index, GLdouble x);
extern void ( * glVertexAttrib1dv) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttrib1f) (GLuint index, GLfloat x);
extern void ( * glVertexAttrib1fv) (GLuint index, const GLfloat* v);
extern void ( * glVertexAttrib1s) (GLuint index, GLshort x);
extern void ( * glVertexAttrib1sv) (GLuint index, const GLshort* v);
extern void ( * glVertexAttrib2d) (GLuint index, GLdouble x, GLdouble y);
extern void ( * glVertexAttrib2dv) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttrib2f) (GLuint index, GLfloat x, GLfloat y);
extern void ( * glVertexAttrib2fv) (GLuint index, const GLfloat* v);
extern void ( * glVertexAttrib2s) (GLuint index, GLshort x, GLshort y);
extern void ( * glVertexAttrib2sv) (GLuint index, const GLshort* v);
extern void ( * glVertexAttrib3d) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void ( * glVertexAttrib3dv) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttrib3f) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void ( * glVertexAttrib3fv) (GLuint index, const GLfloat* v);
extern void ( * glVertexAttrib3s) (GLuint index, GLshort x, GLshort y, GLshort z);
extern void ( * glVertexAttrib3sv) (GLuint index, const GLshort* v);
extern void ( * glVertexAttrib4Nbv) (GLuint index, const GLbyte* v);
extern void ( * glVertexAttrib4Niv) (GLuint index, const GLint* v);
extern void ( * glVertexAttrib4Nsv) (GLuint index, const GLshort* v);
extern void ( * glVertexAttrib4Nub) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void ( * glVertexAttrib4Nubv) (GLuint index, const GLubyte* v);
extern void ( * glVertexAttrib4Nuiv) (GLuint index, const GLuint* v);
extern void ( * glVertexAttrib4Nusv) (GLuint index, const GLushort* v);
extern void ( * glVertexAttrib4bv) (GLuint index, const GLbyte* v);
extern void ( * glVertexAttrib4d) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void ( * glVertexAttrib4dv) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttrib4f) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void ( * glVertexAttrib4fv) (GLuint index, const GLfloat* v);
extern void ( * glVertexAttrib4iv) (GLuint index, const GLint* v);
extern void ( * glVertexAttrib4s) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void ( * glVertexAttrib4sv) (GLuint index, const GLshort* v);
extern void ( * glVertexAttrib4ubv) (GLuint index, const GLubyte* v);
extern void ( * glVertexAttrib4uiv) (GLuint index, const GLuint* v);
extern void ( * glVertexAttrib4usv) (GLuint index, const GLushort* v);
extern void ( * glVertexAttribPointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
extern void ( * glUniformMatrix2x3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void ( * glUniformMatrix2x4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void ( * glUniformMatrix3x2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void ( * glUniformMatrix3x4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void ( * glUniformMatrix4x2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void ( * glUniformMatrix4x3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void ( * glBeginConditionalRender) (GLuint, GLenum);
extern void ( * glBeginTransformFeedback) (GLenum);
extern void ( * glBindFragDataLocation) (GLuint, GLuint, const GLchar*);
extern void ( * glClampColor) (GLenum, GLenum);
extern void ( * glClearBufferfi) (GLenum, GLint, GLfloat, GLint);
extern void ( * glClearBufferfv) (GLenum, GLint, const GLfloat*);
extern void ( * glClearBufferiv) (GLenum, GLint, const GLint*);
extern void ( * glClearBufferuiv) (GLenum, GLint, const GLuint*);
extern void ( * glColorMaski) (GLuint, GLboolean, GLboolean, GLboolean, GLboolean);
extern void ( * glDisablei) (GLenum, GLuint);
extern void ( * glEnablei) (GLenum, GLuint);
extern void ( * glEndConditionalRender) (void);
extern void ( * glEndTransformFeedback) (void);
extern void ( * glGetBooleani_v) (GLenum, GLuint, GLboolean*);
extern GLint ( * glGetFragDataLocation) (GLuint, const GLchar*);
extern const GLubyte* ( * glGetStringi) (GLenum, GLuint);
extern void ( * glGetTexParameterIiv) (GLenum, GLenum, GLint*);
extern void ( * glGetTexParameterIuiv) (GLenum, GLenum, GLuint*);
extern void ( * glGetTransformFeedbackVarying) (GLuint, GLuint, GLint*);
extern void ( * glGetUniformuiv) (GLuint, GLint, GLuint*);
extern void ( * glGetVertexAttribIiv) (GLuint, GLenum, GLint*);
extern void ( * glGetVertexAttribIuiv) (GLuint, GLenum, GLuint*);
extern GLboolean ( * glIsEnabledi) (GLenum, GLuint);
extern void ( * glTexParameterIiv) (GLenum, GLenum, const GLint*);
extern void ( * glTexParameterIuiv) (GLenum, GLenum, const GLuint*);
extern void ( * glTransformFeedbackVaryings) (GLuint, GLsizei, const GLchar **, GLenum);
extern void ( * glUniform1ui) (GLint, GLuint);
extern void ( * glUniform1uiv) (GLint, GLsizei, const GLuint*);
extern void ( * glUniform2ui) (GLint, GLuint, GLuint);
extern void ( * glUniform2uiv) (GLint, GLsizei, const GLuint*);
extern void ( * glUniform3ui) (GLint, GLuint, GLuint, GLuint);
extern void ( * glUniform3uiv) (GLint, GLsizei, const GLuint*);
extern void ( * glUniform4ui) (GLint, GLuint, GLuint, GLuint, GLuint);
extern void ( * glUniform4uiv) (GLint, GLsizei, const GLuint*);
extern void ( * glVertexAttribI1i) (GLuint, GLint);
extern void ( * glVertexAttribI1iv) (GLuint, const GLint*);
extern void ( * glVertexAttribI1ui) (GLuint, GLuint);
extern void ( * glVertexAttribI1uiv) (GLuint, const GLuint*);
extern void ( * glVertexAttribI2i) (GLuint, GLint, GLint);
extern void ( * glVertexAttribI2iv) (GLuint, const GLint*);
extern void ( * glVertexAttribI2ui) (GLuint, GLuint, GLuint);
extern void ( * glVertexAttribI2uiv) (GLuint, const GLuint*);
extern void ( * glVertexAttribI3i) (GLuint, GLint, GLint, GLint);
extern void ( * glVertexAttribI3iv) (GLuint, const GLint*);
extern void ( * glVertexAttribI3ui) (GLuint, GLuint, GLuint, GLuint);
extern void ( * glVertexAttribI3uiv) (GLuint, const GLuint*);
extern void ( * glVertexAttribI4bv) (GLuint, const GLbyte*);
extern void ( * glVertexAttribI4i) (GLuint, GLint, GLint, GLint, GLint);
extern void ( * glVertexAttribI4iv) (GLuint, const GLint*);
extern void ( * glVertexAttribI4sv) (GLuint, const GLshort*);
extern void ( * glVertexAttribI4ubv) (GLuint, const GLubyte*);
extern void ( * glVertexAttribI4ui) (GLuint, GLuint, GLuint, GLuint, GLuint);
extern void ( * glVertexAttribI4uiv) (GLuint, const GLuint*);
extern void ( * glVertexAttribI4usv) (GLuint, const GLushort*);
extern void ( * glVertexAttribIPointer) (GLuint, GLint, GLenum, GLsizei, const GLvoid*);
extern void ( * glDrawArraysInstanced) (GLenum, GLint, GLsizei, GLsizei);
extern void ( * glDrawElementsInstanced) (GLenum, GLsizei, GLenum, const GLvoid*, GLsizei);
extern void ( * glPrimitiveRestartIndex) (GLuint);
extern void ( * glTexBuffer) (GLenum, GLenum, GLuint);
extern void ( * glFramebufferTexture) (GLenum, GLenum, GLuint, GLint);
extern void ( * glGetBufferParameteri64v) (GLenum, GLenum, GLint64 *);
extern void ( * glGetInteger64i_v) (GLenum, GLuint, GLint64 *);
extern void ( * glVertexAttribDivisor) (GLuint index, GLuint divisor);
extern void ( * glBlendEquationSeparatei) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
extern void ( * glBlendEquationi) (GLuint buf, GLenum mode);
extern void ( * glBlendFuncSeparatei) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
extern void ( * glBlendFunci) (GLuint buf, GLenum src, GLenum dst);
extern void ( * glMinSampleShading) (GLclampf value);
extern void ( * glClearDepthf) (GLclampf d);
extern void ( * glDepthRangef) (GLclampf n, GLclampf f);
extern void ( * glGetShaderPrecisionFormat) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint *precision);
extern void ( * glReleaseShaderCompiler) (void);
extern void ( * glShaderBinary) (GLsizei count, const GLuint* shaders, GLenum binaryformat, const GLvoid*binary, GLsizei length);
extern void ( * glBindFragDataLocationIndexed) (GLuint program, GLuint colorNumber, GLuint index, const char * name);
extern GLint ( * glGetFragDataIndex) (GLuint program, const char * name);
extern void ( * glCopyBufferSubData) (GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);
extern void ( * glDrawElementsBaseVertex) (GLenum mode, GLsizei count, GLenum type, void* indices, GLint basevertex);
extern void ( * glDrawElementsInstancedBaseVertex) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLint basevertex);
extern void ( * glDrawRangeElementsBaseVertex) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, void* indices, GLint basevertex);
extern void ( * glMultiDrawElementsBaseVertex) (GLenum mode, GLsizei* count, GLenum type, GLvoid**indices, GLsizei primcount, GLint *basevertex);
extern void ( * glDrawArraysIndirect) (GLenum mode, const void* indirect);
extern void ( * glDrawElementsIndirect) (GLenum mode, GLenum type, const void* indirect);
extern void ( * glBindFramebuffer) (GLenum target, GLuint framebuffer);
extern void ( * glBindRenderbuffer) (GLenum target, GLuint renderbuffer);
extern void ( * glBlitFramebuffer) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern GLenum ( * glCheckFramebufferStatus) (GLenum target);
extern void ( * glDeleteFramebuffers) (GLsizei n, const GLuint* framebuffers);
extern void ( * glDeleteRenderbuffers) (GLsizei n, const GLuint* renderbuffers);
extern void ( * glFramebufferRenderbuffer) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void ( * glFramebufferTextureLayer) (GLenum target,GLenum attachment, GLuint texture,GLint level,GLint layer);
extern void ( * glGenFramebuffers) (GLsizei n, GLuint* framebuffers);
extern void ( * glGenRenderbuffers) (GLsizei n, GLuint* renderbuffers);
extern void ( * glGenerateMipmap) (GLenum target);
extern void ( * glGetFramebufferAttachmentParameteriv) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
extern void ( * glGetRenderbufferParameteriv) (GLenum target, GLenum pname, GLint* params);
extern GLboolean ( * glIsFramebuffer) (GLuint framebuffer);
extern GLboolean ( * glIsRenderbuffer) (GLuint renderbuffer);
extern void ( * glRenderbufferStorage) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void ( * glRenderbufferStorageMultisample) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern void ( * glGetProgramBinary) (GLuint program, GLsizei bufSize, GLsizei* length, GLenum *binaryFormat, GLvoid*binary);
extern void ( * glProgramBinary) (GLuint program, GLenum binaryFormat, const void* binary, GLsizei length);
extern void ( * glProgramParameteri) (GLuint program, GLenum pname, GLint value);
extern void ( * glGetUniformdv) (GLuint program, GLint location, GLdouble* params);
extern void ( * glUniform1d) (GLint location, GLdouble x);
extern void ( * glUniform1dv) (GLint location, GLsizei count, const GLdouble* value);
extern void ( * glUniform2d) (GLint location, GLdouble x, GLdouble y);
extern void ( * glUniform2dv) (GLint location, GLsizei count, const GLdouble* value);
extern void ( * glUniform3d) (GLint location, GLdouble x, GLdouble y, GLdouble z);
extern void ( * glUniform3dv) (GLint location, GLsizei count, const GLdouble* value);
extern void ( * glUniform4d) (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void ( * glUniform4dv) (GLint location, GLsizei count, const GLdouble* value);
extern void ( * glUniformMatrix2dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glUniformMatrix2x3dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glUniformMatrix2x4dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glUniformMatrix3dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glUniformMatrix3x2dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glUniformMatrix3x4dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glUniformMatrix4dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glUniformMatrix4x2dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glUniformMatrix4x3dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glColorSubTable) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
extern void ( * glColorTable) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
extern void ( * glColorTableParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
extern void ( * glColorTableParameteriv) (GLenum target, GLenum pname, const GLint *params);
extern void ( * glConvolutionParameterf) (GLenum target, GLenum pname, GLfloat params);
extern void ( * glConvolutionParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
extern void ( * glConvolutionParameteri) (GLenum target, GLenum pname, GLint params);
extern void ( * glConvolutionParameteriv) (GLenum target, GLenum pname, const GLint *params);
extern void ( * glCopyColorSubTable) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
extern void ( * glCopyColorTable) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern void ( * glGetColorTable) (GLenum target, GLenum format, GLenum type, GLvoid *table);
extern void ( * glGetColorTableParameterfv) (GLenum target, GLenum pname, GLfloat *params);
extern void ( * glGetColorTableParameteriv) (GLenum target, GLenum pname, GLint *params);
extern void ( * glGetConvolutionFilter) (GLenum target, GLenum format, GLenum type, GLvoid *image);
extern void ( * glGetConvolutionParameterfv) (GLenum target, GLenum pname, GLfloat *params);
extern void ( * glGetConvolutionParameteriv) (GLenum target, GLenum pname, GLint *params);
extern void ( * glGetHistogram) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
extern void ( * glGetHistogramParameterfv) (GLenum target, GLenum pname, GLfloat *params);
extern void ( * glGetHistogramParameteriv) (GLenum target, GLenum pname, GLint *params);
extern void ( * glGetMinmax) (GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values);
extern void ( * glGetMinmaxParameterfv) (GLenum target, GLenum pname, GLfloat *params);
extern void ( * glGetMinmaxParameteriv) (GLenum target, GLenum pname, GLint *params);
extern void ( * glGetSeparableFilter) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
extern void ( * glHistogram) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
extern void ( * glMinmax) (GLenum target, GLenum internalformat, GLboolean sink);
extern void ( * glResetHistogram) (GLenum target);
extern void ( * glResetMinmax) (GLenum target);
extern void ( * glFlushMappedBufferRange) (GLenum target, GLintptr offset, GLsizeiptr length);
extern GLvoid * ( * glMapBufferRange) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
extern void ( * glProvokingVertex) (GLenum mode);
extern void ( * glBindSampler) (GLuint unit, GLuint sampler);
extern void ( * glDeleteSamplers) (GLsizei count, const GLuint * samplers);
extern void ( * glGenSamplers) (GLsizei count, GLuint* samplers);
extern void ( * glGetSamplerParameterIiv) (GLuint sampler, GLenum pname, GLint* params);
extern void ( * glGetSamplerParameterIuiv) (GLuint sampler, GLenum pname, GLuint* params);
extern void ( * glGetSamplerParameterfv) (GLuint sampler, GLenum pname, GLfloat* params);
extern void ( * glGetSamplerParameteriv) (GLuint sampler, GLenum pname, GLint* params);
extern GLboolean ( * glIsSampler) (GLuint sampler);
extern void ( * glSamplerParameterIiv) (GLuint sampler, GLenum pname, const GLint* params);
extern void ( * glSamplerParameterIuiv) (GLuint sampler, GLenum pname, const GLuint* params);
extern void ( * glSamplerParameterf) (GLuint sampler, GLenum pname, GLfloat param);
extern void ( * glSamplerParameterfv) (GLuint sampler, GLenum pname, const GLfloat* params);
extern void ( * glSamplerParameteri) (GLuint sampler, GLenum pname, GLint param);
extern void ( * glSamplerParameteriv) (GLuint sampler, GLenum pname, const GLint* params);
extern void ( * glActiveShaderProgram) (GLuint pipeline, GLuint program);
extern void ( * glBindProgramPipeline) (GLuint pipeline);
extern GLuint ( * glCreateShaderProgramv) (GLenum type, GLsizei count, const char ** strings);
extern void ( * glDeleteProgramPipelines) (GLsizei n, const GLuint* pipelines);
extern void ( * glGenProgramPipelines) (GLsizei n, GLuint* pipelines);
extern void ( * glGetProgramPipelineInfoLog) (GLuint pipeline, GLsizei bufSize, GLsizei* length, char *infoLog);
extern void ( * glGetProgramPipelineiv) (GLuint pipeline, GLenum pname, GLint* params);
extern GLboolean ( * glIsProgramPipeline) (GLuint pipeline);
extern void ( * glProgramUniform1d) (GLuint program, GLint location, GLdouble x);
extern void ( * glProgramUniform1dv) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
extern void ( * glProgramUniform1f) (GLuint program, GLint location, GLfloat x);
extern void ( * glProgramUniform1fv) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
extern void ( * glProgramUniform1i) (GLuint program, GLint location, GLint x);
extern void ( * glProgramUniform1iv) (GLuint program, GLint location, GLsizei count, const GLint* value);
extern void ( * glProgramUniform1ui) (GLuint program, GLint location, GLuint x);
extern void ( * glProgramUniform1uiv) (GLuint program, GLint location, GLsizei count, const GLuint* value);
extern void ( * glProgramUniform2d) (GLuint program, GLint location, GLdouble x, GLdouble y);
extern void ( * glProgramUniform2dv) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
extern void ( * glProgramUniform2f) (GLuint program, GLint location, GLfloat x, GLfloat y);
extern void ( * glProgramUniform2fv) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
extern void ( * glProgramUniform2i) (GLuint program, GLint location, GLint x, GLint y);
extern void ( * glProgramUniform2iv) (GLuint program, GLint location, GLsizei count, const GLint* value);
extern void ( * glProgramUniform2ui) (GLuint program, GLint location, GLuint x, GLuint y);
extern void ( * glProgramUniform2uiv) (GLuint program, GLint location, GLsizei count, const GLuint* value);
extern void ( * glProgramUniform3d) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
extern void ( * glProgramUniform3dv) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
extern void ( * glProgramUniform3f) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z);
extern void ( * glProgramUniform3fv) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
extern void ( * glProgramUniform3i) (GLuint program, GLint location, GLint x, GLint y, GLint z);
extern void ( * glProgramUniform3iv) (GLuint program, GLint location, GLsizei count, const GLint* value);
extern void ( * glProgramUniform3ui) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z);
extern void ( * glProgramUniform3uiv) (GLuint program, GLint location, GLsizei count, const GLuint* value);
extern void ( * glProgramUniform4d) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void ( * glProgramUniform4dv) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
extern void ( * glProgramUniform4f) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void ( * glProgramUniform4fv) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
extern void ( * glProgramUniform4i) (GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w);
extern void ( * glProgramUniform4iv) (GLuint program, GLint location, GLsizei count, const GLint* value);
extern void ( * glProgramUniform4ui) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
extern void ( * glProgramUniform4uiv) (GLuint program, GLint location, GLsizei count, const GLuint* value);
extern void ( * glProgramUniformMatrix2dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glProgramUniformMatrix2fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glProgramUniformMatrix2x3dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glProgramUniformMatrix2x3fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glProgramUniformMatrix2x4dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glProgramUniformMatrix2x4fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glProgramUniformMatrix3dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glProgramUniformMatrix3fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glProgramUniformMatrix3x2dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glProgramUniformMatrix3x2fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glProgramUniformMatrix3x4dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glProgramUniformMatrix3x4fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glProgramUniformMatrix4dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glProgramUniformMatrix4fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glProgramUniformMatrix4x2dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glProgramUniformMatrix4x2fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glProgramUniformMatrix4x3dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
extern void ( * glProgramUniformMatrix4x3fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void ( * glUseProgramStages) (GLuint pipeline, GLbitfield stages, GLuint program);
extern void ( * glValidateProgramPipeline) (GLuint pipeline);
extern void ( * glGetActiveSubroutineName) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, char *name);
extern void ( * glGetActiveSubroutineUniformName) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, char *name);
extern void ( * glGetActiveSubroutineUniformiv) (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint* values);
extern void ( * glGetProgramStageiv) (GLuint program, GLenum shadertype, GLenum pname, GLint* values);
extern GLuint ( * glGetSubroutineIndex) (GLuint program, GLenum shadertype, const char* name);
extern GLint ( * glGetSubroutineUniformLocation) (GLuint program, GLenum shadertype, const char* name);
extern void ( * glGetUniformSubroutineuiv) (GLenum shadertype, GLint location, GLuint* params);
extern void ( * glUniformSubroutinesuiv) (GLenum shadertype, GLsizei count, const GLuint* indices);
extern GLenum ( * glClientWaitSync) (GLsync GLsync,GLbitfield flags,GLuint64 timeout);
extern void ( * glDeleteSync) (GLsync GLsync);
extern GLsync ( * glFenceSync) (GLenum condition,GLbitfield flags);
extern void ( * glGetInteger64v) (GLenum pname, GLint64* params);
extern void ( * glGetSynciv) (GLsync GLsync,GLenum pname,GLsizei bufSize,GLsizei* length, GLint *values);
extern GLboolean ( * glIsSync) (GLsync GLsync);
extern void ( * glWaitSync) (GLsync GLsync,GLbitfield flags,GLuint64 timeout);
extern void ( * glPatchParameterfv) (GLenum pname, const GLfloat* values);
extern void ( * glPatchParameteri) (GLenum pname, GLint value);
extern void ( * glGetMultisamplefv) (GLenum pname, GLuint index, GLfloat* val);
extern void ( * glSampleMaski) (GLuint index, GLbitfield mask);
extern void ( * glTexImage2DMultisample) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern void ( * glTexImage3DMultisample) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern void ( * glGetQueryObjecti64v) (GLuint id, GLenum pname, GLint64* params);
extern void ( * glGetQueryObjectui64v) (GLuint id, GLenum pname, GLuint64* params);
extern void ( * glQueryCounter) (GLuint id, GLenum target);
extern void ( * glBindTransformFeedback) (GLenum target, GLuint id);
extern void ( * glDeleteTransformFeedbacks) (GLsizei n, const GLuint* ids);
extern void ( * glDrawTransformFeedback) (GLenum mode, GLuint id);
extern void ( * glGenTransformFeedbacks) (GLsizei n, GLuint* ids);
extern GLboolean ( * glIsTransformFeedback) (GLuint id);
extern void ( * glPauseTransformFeedback) (void);
extern void ( * glResumeTransformFeedback) (void);
extern void ( * glBeginQueryIndexed) (GLenum target, GLuint index, GLuint id);
extern void ( * glDrawTransformFeedbackStream) (GLenum mode, GLuint id, GLuint stream);
extern void ( * glEndQueryIndexed) (GLenum target, GLuint index);
extern void ( * glGetQueryIndexediv) (GLenum target, GLuint index, GLenum pname, GLint* params);
extern void ( * glBindBufferBase) (GLenum target, GLuint index, GLuint buffer);
extern void ( * glBindBufferRange) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern void ( * glGetActiveUniformBlockName) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, char* uniformBlockName);
extern void ( * glGetActiveUniformBlockiv) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
extern void ( * glGetActiveUniformName) (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, char* uniformName);
extern void ( * glGetActiveUniformsiv) (GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
extern void ( * glGetIntegeri_v) (GLenum target, GLuint index, GLint* data);
extern GLuint ( * glGetUniformBlockIndex) (GLuint program, const char* uniformBlockName);
extern void ( * glGetUniformIndices) (GLuint program, GLsizei uniformCount, const char** uniformNames, GLuint* uniformIndices);
extern void ( * glUniformBlockBinding) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
extern void ( * glBindVertexArray) (GLuint array);
extern void ( * glDeleteVertexArrays) (GLsizei n, const GLuint* arrays);
extern void ( * glGenVertexArrays) (GLsizei n, GLuint* arrays);
extern GLboolean ( * glIsVertexArray) (GLuint array);
extern void ( * glGetVertexAttribLdv) (GLuint index, GLenum pname, GLdouble* params);
extern void ( * glVertexAttribL1d) (GLuint index, GLdouble x);
extern void ( * glVertexAttribL1dv) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttribL2d) (GLuint index, GLdouble x, GLdouble y);
extern void ( * glVertexAttribL2dv) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttribL3d) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void ( * glVertexAttribL3dv) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttribL4d) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void ( * glVertexAttribL4dv) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttribLPointer) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
extern void ( * glColorP3ui) (GLenum type, GLuint color);
extern void ( * glColorP3uiv) (GLenum type, const GLuint* color);
extern void ( * glColorP4ui) (GLenum type, GLuint color);
extern void ( * glColorP4uiv) (GLenum type, const GLuint* color);
extern void ( * glMultiTexCoordP1ui) (GLenum texture, GLenum type, GLuint coords);
extern void ( * glMultiTexCoordP1uiv) (GLenum texture, GLenum type, const GLuint* coords);
extern void ( * glMultiTexCoordP2ui) (GLenum texture, GLenum type, GLuint coords);
extern void ( * glMultiTexCoordP2uiv) (GLenum texture, GLenum type, const GLuint* coords);
extern void ( * glMultiTexCoordP3ui) (GLenum texture, GLenum type, GLuint coords);
extern void ( * glMultiTexCoordP3uiv) (GLenum texture, GLenum type, const GLuint* coords);
extern void ( * glMultiTexCoordP4ui) (GLenum texture, GLenum type, GLuint coords);
extern void ( * glMultiTexCoordP4uiv) (GLenum texture, GLenum type, const GLuint* coords);
extern void ( * glNormalP3ui) (GLenum type, GLuint coords);
extern void ( * glNormalP3uiv) (GLenum type, const GLuint* coords);
extern void ( * glSecondaryColorP3ui) (GLenum type, GLuint color);
extern void ( * glSecondaryColorP3uiv) (GLenum type, const GLuint* color);
extern void ( * glTexCoordP1ui) (GLenum type, GLuint coords);
extern void ( * glTexCoordP1uiv) (GLenum type, const GLuint* coords);
extern void ( * glTexCoordP2ui) (GLenum type, GLuint coords);
extern void ( * glTexCoordP2uiv) (GLenum type, const GLuint* coords);
extern void ( * glTexCoordP3ui) (GLenum type, GLuint coords);
extern void ( * glTexCoordP3uiv) (GLenum type, const GLuint* coords);
extern void ( * glTexCoordP4ui) (GLenum type, GLuint coords);
extern void ( * glTexCoordP4uiv) (GLenum type, const GLuint* coords);
extern void ( * glVertexAttribP1ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void ( * glVertexAttribP1uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
extern void ( * glVertexAttribP2ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void ( * glVertexAttribP2uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
extern void ( * glVertexAttribP3ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void ( * glVertexAttribP3uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
extern void ( * glVertexAttribP4ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void ( * glVertexAttribP4uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
extern void ( * glVertexP2ui) (GLenum type, GLuint value);
extern void ( * glVertexP2uiv) (GLenum type, const GLuint* value);
extern void ( * glVertexP3ui) (GLenum type, GLuint value);
extern void ( * glVertexP3uiv) (GLenum type, const GLuint* value);
extern void ( * glVertexP4ui) (GLenum type, GLuint value);
extern void ( * glVertexP4uiv) (GLenum type, const GLuint* value);
extern void ( * glDepthRangeArrayv) (GLuint first, GLsizei count, const GLclampd * v);
extern void ( * glDepthRangeIndexed) (GLuint index, GLclampd n, GLclampd f);
extern void ( * glGetDoublei_v) (GLenum target, GLuint index, GLdouble* data);
extern void ( * glGetFloati_v) (GLenum target, GLuint index, GLfloat* data);
extern void ( * glScissorArrayv) (GLuint first, GLsizei count, const GLint * v);
extern void ( * glScissorIndexed) (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
extern void ( * glScissorIndexedv) (GLuint index, const GLint * v);
extern void ( * glViewportArrayv) (GLuint first, GLsizei count, const GLfloat * v);
extern void ( * glViewportIndexedf) (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
extern void ( * glViewportIndexedfv) (GLuint index, const GLfloat * v);
extern void ( * glBeginConditionalRenderNV) (GLuint id, GLenum mode);
extern void ( * glEndConditionalRenderNV) (void);
extern void ( * glCopyImageSubDataNV) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);
extern void ( * glClearDepthdNV) (GLdouble depth);
extern void ( * glDepthBoundsdNV) (GLdouble zmin, GLdouble zmax);
extern void ( * glDepthRangedNV) (GLdouble zNear, GLdouble zFar);
extern void ( * glEvalMapsNV) (GLenum target, GLenum mode);
extern void ( * glGetMapAttribParameterfvNV) (GLenum target, GLuint index, GLenum pname, GLfloat* params);
extern void ( * glGetMapAttribParameterivNV) (GLenum target, GLuint index, GLenum pname, GLint* params);
extern void ( * glGetMapControlPointsNV) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, void* points);
extern void ( * glGetMapParameterfvNV) (GLenum target, GLenum pname, GLfloat* params);
extern void ( * glGetMapParameterivNV) (GLenum target, GLenum pname, GLint* params);
extern void ( * glMapControlPointsNV) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const void* points);
extern void ( * glMapParameterfvNV) (GLenum target, GLenum pname, const GLfloat* params);
extern void ( * glMapParameterivNV) (GLenum target, GLenum pname, const GLint* params);
extern void ( * glGetMultisamplefvNV) (GLenum pname, GLuint index, GLfloat* val);
extern void ( * glSampleMaskIndexedNV) (GLuint index, GLbitfield mask);
extern void ( * glTexRenderbufferNV) (GLenum target, GLuint renderbuffer);
extern void ( * glDeleteFencesNV) (GLsizei n, const GLuint* fences);
extern void ( * glFinishFenceNV) (GLuint fence);
extern void ( * glGenFencesNV) (GLsizei n, GLuint* fences);
extern void ( * glGetFenceivNV) (GLuint fence, GLenum pname, GLint* params);
extern GLboolean ( * glIsFenceNV) (GLuint fence);
extern void ( * glSetFenceNV) (GLuint fence, GLenum condition);
extern GLboolean ( * glTestFenceNV) (GLuint fence);
extern void ( * glGetProgramNamedParameterdvNV) (GLuint id, GLsizei len, const GLubyte* name, GLdouble *params);
extern void ( * glGetProgramNamedParameterfvNV) (GLuint id, GLsizei len, const GLubyte* name, GLfloat *params);
extern void ( * glProgramNamedParameter4dNV) (GLuint id, GLsizei len, const GLubyte* name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void ( * glProgramNamedParameter4dvNV) (GLuint id, GLsizei len, const GLubyte* name, const GLdouble v[]);
extern void ( * glProgramNamedParameter4fNV) (GLuint id, GLsizei len, const GLubyte* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void ( * glProgramNamedParameter4fvNV) (GLuint id, GLsizei len, const GLubyte* name, const GLfloat v[]);
extern void ( * glRenderbufferStorageMultisampleCoverageNV) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
extern void ( * glProgramVertexLimitNV) (GLenum target, GLint limit);
extern void ( * glProgramEnvParameterI4iNV) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
extern void ( * glProgramEnvParameterI4ivNV) (GLenum target, GLuint index, const GLint *params);
extern void ( * glProgramEnvParameterI4uiNV) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern void ( * glProgramEnvParameterI4uivNV) (GLenum target, GLuint index, const GLuint *params);
extern void ( * glProgramEnvParametersI4ivNV) (GLenum target, GLuint index, GLsizei count, const GLint *params);
extern void ( * glProgramEnvParametersI4uivNV) (GLenum target, GLuint index, GLsizei count, const GLuint *params);
extern void ( * glProgramLocalParameterI4iNV) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
extern void ( * glProgramLocalParameterI4ivNV) (GLenum target, GLuint index, const GLint *params);
extern void ( * glProgramLocalParameterI4uiNV) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern void ( * glProgramLocalParameterI4uivNV) (GLenum target, GLuint index, const GLuint *params);
extern void ( * glProgramLocalParametersI4ivNV) (GLenum target, GLuint index, GLsizei count, const GLint *params);
extern void ( * glProgramLocalParametersI4uivNV) (GLenum target, GLuint index, GLsizei count, const GLuint *params);
extern void ( * glGetUniformi64vNV) (GLuint program, GLint location, GLint64EXT* params);
extern void ( * glGetUniformui64vNV) (GLuint program, GLint location, GLuint64EXT* params);
extern void ( * glProgramUniform1i64NV) (GLuint program, GLint location, GLint64EXT x);
extern void ( * glProgramUniform1i64vNV) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
extern void ( * glProgramUniform1ui64NV) (GLuint program, GLint location, GLuint64EXT x);
extern void ( * glProgramUniform1ui64vNV) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
extern void ( * glProgramUniform2i64NV) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y);
extern void ( * glProgramUniform2i64vNV) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
extern void ( * glProgramUniform2ui64NV) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y);
extern void ( * glProgramUniform2ui64vNV) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
extern void ( * glProgramUniform3i64NV) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
extern void ( * glProgramUniform3i64vNV) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
extern void ( * glProgramUniform3ui64NV) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
extern void ( * glProgramUniform3ui64vNV) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
extern void ( * glProgramUniform4i64NV) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
extern void ( * glProgramUniform4i64vNV) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
extern void ( * glProgramUniform4ui64NV) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
extern void ( * glProgramUniform4ui64vNV) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
extern void ( * glUniform1i64NV) (GLint location, GLint64EXT x);
extern void ( * glUniform1i64vNV) (GLint location, GLsizei count, const GLint64EXT* value);
extern void ( * glUniform1ui64NV) (GLint location, GLuint64EXT x);
extern void ( * glUniform1ui64vNV) (GLint location, GLsizei count, const GLuint64EXT* value);
extern void ( * glUniform2i64NV) (GLint location, GLint64EXT x, GLint64EXT y);
extern void ( * glUniform2i64vNV) (GLint location, GLsizei count, const GLint64EXT* value);
extern void ( * glUniform2ui64NV) (GLint location, GLuint64EXT x, GLuint64EXT y);
extern void ( * glUniform2ui64vNV) (GLint location, GLsizei count, const GLuint64EXT* value);
extern void ( * glUniform3i64NV) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
extern void ( * glUniform3i64vNV) (GLint location, GLsizei count, const GLint64EXT* value);
extern void ( * glUniform3ui64NV) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
extern void ( * glUniform3ui64vNV) (GLint location, GLsizei count, const GLuint64EXT* value);
extern void ( * glUniform4i64NV) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
extern void ( * glUniform4i64vNV) (GLint location, GLsizei count, const GLint64EXT* value);
extern void ( * glUniform4ui64NV) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
extern void ( * glUniform4ui64vNV) (GLint location, GLsizei count, const GLuint64EXT* value);
extern void ( * glColor3hNV) (GLhalf red, GLhalf green, GLhalf blue);
extern void ( * glColor3hvNV) (const GLhalf* v);
extern void ( * glColor4hNV) (GLhalf red, GLhalf green, GLhalf blue, GLhalf alpha);
extern void ( * glColor4hvNV) (const GLhalf* v);
extern void ( * glFogCoordhNV) (GLhalf fog);
extern void ( * glFogCoordhvNV) (const GLhalf* fog);
extern void ( * glMultiTexCoord1hNV) (GLenum target, GLhalf s);
extern void ( * glMultiTexCoord1hvNV) (GLenum target, const GLhalf* v);
extern void ( * glMultiTexCoord2hNV) (GLenum target, GLhalf s, GLhalf t);
extern void ( * glMultiTexCoord2hvNV) (GLenum target, const GLhalf* v);
extern void ( * glMultiTexCoord3hNV) (GLenum target, GLhalf s, GLhalf t, GLhalf r);
extern void ( * glMultiTexCoord3hvNV) (GLenum target, const GLhalf* v);
extern void ( * glMultiTexCoord4hNV) (GLenum target, GLhalf s, GLhalf t, GLhalf r, GLhalf q);
extern void ( * glMultiTexCoord4hvNV) (GLenum target, const GLhalf* v);
extern void ( * glNormal3hNV) (GLhalf nx, GLhalf ny, GLhalf nz);
extern void ( * glNormal3hvNV) (const GLhalf* v);
extern void ( * glSecondaryColor3hNV) (GLhalf red, GLhalf green, GLhalf blue);
extern void ( * glSecondaryColor3hvNV) (const GLhalf* v);
extern void ( * glTexCoord1hNV) (GLhalf s);
extern void ( * glTexCoord1hvNV) (const GLhalf* v);
extern void ( * glTexCoord2hNV) (GLhalf s, GLhalf t);
extern void ( * glTexCoord2hvNV) (const GLhalf* v);
extern void ( * glTexCoord3hNV) (GLhalf s, GLhalf t, GLhalf r);
extern void ( * glTexCoord3hvNV) (const GLhalf* v);
extern void ( * glTexCoord4hNV) (GLhalf s, GLhalf t, GLhalf r, GLhalf q);
extern void ( * glTexCoord4hvNV) (const GLhalf* v);
extern void ( * glVertex2hNV) (GLhalf x, GLhalf y);
extern void ( * glVertex2hvNV) (const GLhalf* v);
extern void ( * glVertex3hNV) (GLhalf x, GLhalf y, GLhalf z);
extern void ( * glVertex3hvNV) (const GLhalf* v);
extern void ( * glVertex4hNV) (GLhalf x, GLhalf y, GLhalf z, GLhalf w);
extern void ( * glVertex4hvNV) (const GLhalf* v);
extern void ( * glVertexAttrib1hNV) (GLuint index, GLhalf x);
extern void ( * glVertexAttrib1hvNV) (GLuint index, const GLhalf* v);
extern void ( * glVertexAttrib2hNV) (GLuint index, GLhalf x, GLhalf y);
extern void ( * glVertexAttrib2hvNV) (GLuint index, const GLhalf* v);
extern void ( * glVertexAttrib3hNV) (GLuint index, GLhalf x, GLhalf y, GLhalf z);
extern void ( * glVertexAttrib3hvNV) (GLuint index, const GLhalf* v);
extern void ( * glVertexAttrib4hNV) (GLuint index, GLhalf x, GLhalf y, GLhalf z, GLhalf w);
extern void ( * glVertexAttrib4hvNV) (GLuint index, const GLhalf* v);
extern void ( * glVertexAttribs1hvNV) (GLuint index, GLsizei n, const GLhalf* v);
extern void ( * glVertexAttribs2hvNV) (GLuint index, GLsizei n, const GLhalf* v);
extern void ( * glVertexAttribs3hvNV) (GLuint index, GLsizei n, const GLhalf* v);
extern void ( * glVertexAttribs4hvNV) (GLuint index, GLsizei n, const GLhalf* v);
extern void ( * glVertexWeighthNV) (GLhalf weight);
extern void ( * glVertexWeighthvNV) (const GLhalf* weight);
extern void ( * glBeginOcclusionQueryNV) (GLuint id);
extern void ( * glDeleteOcclusionQueriesNV) (GLsizei n, const GLuint* ids);
extern void ( * glEndOcclusionQueryNV) (void);
extern void ( * glGenOcclusionQueriesNV) (GLsizei n, GLuint* ids);
extern void ( * glGetOcclusionQueryivNV) (GLuint id, GLenum pname, GLint* params);
extern void ( * glGetOcclusionQueryuivNV) (GLuint id, GLenum pname, GLuint* params);
extern GLboolean ( * glIsOcclusionQueryNV) (GLuint id);
extern void ( * glProgramBufferParametersIivNV) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLint *params);
extern void ( * glProgramBufferParametersIuivNV) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLuint *params);
extern void ( * glProgramBufferParametersfvNV) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLfloat *params);
extern void ( * glFlushPixelDataRangeNV) (GLenum target);
extern void ( * glPixelDataRangeNV) (GLenum target, GLsizei length, void* pointer);
extern void ( * glPointParameteriNV) (GLenum pname, GLint param);
extern void ( * glPointParameterivNV) (GLenum pname, const GLint* params);
extern void ( * glGetVideoi64vNV) (GLuint video_slot, GLenum pname, GLint64EXT* params);
extern void ( * glGetVideoivNV) (GLuint video_slot, GLenum pname, GLint* params);
extern void ( * glGetVideoui64vNV) (GLuint video_slot, GLenum pname, GLuint64EXT* params);
extern void ( * glGetVideouivNV) (GLuint video_slot, GLenum pname, GLuint* params);
extern void ( * glPresentFrameDualFillNV) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3);
extern void ( * glPresentFrameKeyedNV) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1);
extern void ( * glPrimitiveRestartIndexNV) (GLuint index);
extern void ( * glPrimitiveRestartNV) (void);
extern void ( * glCombinerInputNV) (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
extern void ( * glCombinerOutputNV) (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
extern void ( * glCombinerParameterfNV) (GLenum pname, GLfloat param);
extern void ( * glCombinerParameterfvNV) (GLenum pname, const GLfloat* params);
extern void ( * glCombinerParameteriNV) (GLenum pname, GLint param);
extern void ( * glCombinerParameterivNV) (GLenum pname, const GLint* params);
extern void ( * glFinalCombinerInputNV) (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
extern void ( * glGetCombinerInputParameterfvNV) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat* params);
extern void ( * glGetCombinerInputParameterivNV) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint* params);
extern void ( * glGetCombinerOutputParameterfvNV) (GLenum stage, GLenum portion, GLenum pname, GLfloat* params);
extern void ( * glGetCombinerOutputParameterivNV) (GLenum stage, GLenum portion, GLenum pname, GLint* params);
extern void ( * glGetFinalCombinerInputParameterfvNV) (GLenum variable, GLenum pname, GLfloat* params);
extern void ( * glGetFinalCombinerInputParameterivNV) (GLenum variable, GLenum pname, GLint* params);
extern void ( * glCombinerStageParameterfvNV) (GLenum stage, GLenum pname, const GLfloat* params);
extern void ( * glGetCombinerStageParameterfvNV) (GLenum stage, GLenum pname, GLfloat* params);
extern void ( * glGetBufferParameterui64vNV) (GLenum target, GLenum pname, GLuint64EXT* params);
extern void ( * glGetIntegerui64vNV) (GLenum value, GLuint64EXT* result);
extern void ( * glGetNamedBufferParameterui64vNV) (GLuint buffer, GLenum pname, GLuint64EXT* params);
extern GLboolean ( * glIsBufferResidentNV) (GLenum target);
extern GLboolean ( * glIsNamedBufferResidentNV) (GLuint buffer);
extern void ( * glMakeBufferNonResidentNV) (GLenum target);
extern void ( * glMakeBufferResidentNV) (GLenum target, GLenum access);
extern void ( * glMakeNamedBufferNonResidentNV) (GLuint buffer);
extern void ( * glMakeNamedBufferResidentNV) (GLuint buffer, GLenum access);
extern void ( * glProgramUniformui64NV) (GLuint program, GLint location, GLuint64EXT value);
extern void ( * glProgramUniformui64vNV) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
extern void ( * glUniformui64NV) (GLint location, GLuint64EXT value);
extern void ( * glUniformui64vNV) (GLint location, GLsizei count, const GLuint64EXT* value);
extern void ( * glTextureBarrierNV) (void);
extern void ( * glActiveVaryingNV) (GLuint program, const GLchar *name);
extern void ( * glBeginTransformFeedbackNV) (GLenum primitiveMode);
extern void ( * glBindBufferBaseNV) (GLenum target, GLuint index, GLuint buffer);
extern void ( * glBindBufferOffsetNV) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
extern void ( * glBindBufferRangeNV) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern void ( * glEndTransformFeedbackNV) (void);
extern void ( * glGetActiveVaryingNV) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
extern void ( * glGetTransformFeedbackVaryingNV) (GLuint program, GLuint index, GLint *location);
extern GLint ( * glGetVaryingLocationNV) (GLuint program, const GLchar *name);
extern void ( * glTransformFeedbackAttribsNV) (GLuint count, const GLint *attribs, GLenum bufferMode);
extern void ( * glTransformFeedbackVaryingsNV) (GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode);
extern void ( * glBindTransformFeedbackNV) (GLenum target, GLuint id);
extern void ( * glDeleteTransformFeedbacksNV) (GLsizei n, const GLuint* ids);
extern void ( * glDrawTransformFeedbackNV) (GLenum mode, GLuint id);
extern void ( * glGenTransformFeedbacksNV) (GLsizei n, GLuint* ids);
extern GLboolean ( * glIsTransformFeedbackNV) (GLuint id);
extern void ( * glPauseTransformFeedbackNV) (void);
extern void ( * glResumeTransformFeedbackNV) (void);
extern void ( * glVDPAUFiniNV) (void);
extern void ( * glVDPAUGetSurfaceivNV) (GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei* length, GLint *values);
extern void ( * glVDPAUInitNV) (const void* vdpDevice, const GLvoid*getProcAddress);
extern void ( * glVDPAUIsSurfaceNV) (GLvdpauSurfaceNV surface);
extern void ( * glVDPAUMapSurfacesNV) (GLsizei numSurfaces, const GLvdpauSurfaceNV* surfaces);
extern GLvdpauSurfaceNV ( * glVDPAURegisterOutputSurfaceNV) (const void* vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
extern GLvdpauSurfaceNV ( * glVDPAURegisterVideoSurfaceNV) (const void* vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
extern void ( * glVDPAUSurfaceAccessNV) (GLvdpauSurfaceNV surface, GLenum access);
extern void ( * glVDPAUUnmapSurfacesNV) (GLsizei numSurface, const GLvdpauSurfaceNV* surfaces);
extern void ( * glVDPAUUnregisterSurfaceNV) (GLvdpauSurfaceNV surface);
extern void ( * glFlushVertexArrayRangeNV) (void);
extern void ( * glVertexArrayRangeNV) (GLsizei length, void* pointer);
extern void ( * glGetVertexAttribLi64vNV) (GLuint index, GLenum pname, GLint64EXT* params);
extern void ( * glGetVertexAttribLui64vNV) (GLuint index, GLenum pname, GLuint64EXT* params);
extern void ( * glVertexAttribL1i64NV) (GLuint index, GLint64EXT x);
extern void ( * glVertexAttribL1i64vNV) (GLuint index, const GLint64EXT* v);
extern void ( * glVertexAttribL1ui64NV) (GLuint index, GLuint64EXT x);
extern void ( * glVertexAttribL1ui64vNV) (GLuint index, const GLuint64EXT* v);
extern void ( * glVertexAttribL2i64NV) (GLuint index, GLint64EXT x, GLint64EXT y);
extern void ( * glVertexAttribL2i64vNV) (GLuint index, const GLint64EXT* v);
extern void ( * glVertexAttribL2ui64NV) (GLuint index, GLuint64EXT x, GLuint64EXT y);
extern void ( * glVertexAttribL2ui64vNV) (GLuint index, const GLuint64EXT* v);
extern void ( * glVertexAttribL3i64NV) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z);
extern void ( * glVertexAttribL3i64vNV) (GLuint index, const GLint64EXT* v);
extern void ( * glVertexAttribL3ui64NV) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
extern void ( * glVertexAttribL3ui64vNV) (GLuint index, const GLuint64EXT* v);
extern void ( * glVertexAttribL4i64NV) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
extern void ( * glVertexAttribL4i64vNV) (GLuint index, const GLint64EXT* v);
extern void ( * glVertexAttribL4ui64NV) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
extern void ( * glVertexAttribL4ui64vNV) (GLuint index, const GLuint64EXT* v);
extern void ( * glVertexAttribLFormatNV) (GLuint index, GLint size, GLenum type, GLsizei stride);
extern void ( * glBufferAddressRangeNV) (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
extern void ( * glColorFormatNV) (GLint size, GLenum type, GLsizei stride);
extern void ( * glEdgeFlagFormatNV) (GLsizei stride);
extern void ( * glFogCoordFormatNV) (GLenum type, GLsizei stride);
extern void ( * glGetIntegerui64i_vNV) (GLenum value, GLuint index, GLuint64EXT result[]);
extern void ( * glIndexFormatNV) (GLenum type, GLsizei stride);
extern void ( * glNormalFormatNV) (GLenum type, GLsizei stride);
extern void ( * glSecondaryColorFormatNV) (GLint size, GLenum type, GLsizei stride);
extern void ( * glTexCoordFormatNV) (GLint size, GLenum type, GLsizei stride);
extern void ( * glVertexAttribFormatNV) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
extern void ( * glVertexAttribIFormatNV) (GLuint index, GLint size, GLenum type, GLsizei stride);
extern void ( * glVertexFormatNV) (GLint size, GLenum type, GLsizei stride);
extern GLboolean ( * glAreProgramsResidentNV) (GLsizei n, const GLuint* ids, GLboolean *residences);
extern void ( * glBindProgramNV) (GLenum target, GLuint id);
extern void ( * glDeleteProgramsNV) (GLsizei n, const GLuint* ids);
extern void ( * glExecuteProgramNV) (GLenum target, GLuint id, const GLfloat* params);
extern void ( * glGenProgramsNV) (GLsizei n, GLuint* ids);
extern void ( * glGetProgramParameterdvNV) (GLenum target, GLuint index, GLenum pname, GLdouble* params);
extern void ( * glGetProgramParameterfvNV) (GLenum target, GLuint index, GLenum pname, GLfloat* params);
extern void ( * glGetProgramStringNV) (GLuint id, GLenum pname, GLubyte* program);
extern void ( * glGetProgramivNV) (GLuint id, GLenum pname, GLint* params);
extern void ( * glGetTrackMatrixivNV) (GLenum target, GLuint address, GLenum pname, GLint* params);
extern void ( * glGetVertexAttribPointervNV) (GLuint index, GLenum pname, GLvoid** pointer);
extern void ( * glGetVertexAttribdvNV) (GLuint index, GLenum pname, GLdouble* params);
extern void ( * glGetVertexAttribfvNV) (GLuint index, GLenum pname, GLfloat* params);
extern void ( * glGetVertexAttribivNV) (GLuint index, GLenum pname, GLint* params);
extern GLboolean ( * glIsProgramNV) (GLuint id);
extern void ( * glLoadProgramNV) (GLenum target, GLuint id, GLsizei len, const GLubyte* program);
extern void ( * glProgramParameter4dNV) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void ( * glProgramParameter4dvNV) (GLenum target, GLuint index, const GLdouble* params);
extern void ( * glProgramParameter4fNV) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void ( * glProgramParameter4fvNV) (GLenum target, GLuint index, const GLfloat* params);
extern void ( * glProgramParameters4dvNV) (GLenum target, GLuint index, GLsizei num, const GLdouble* params);
extern void ( * glProgramParameters4fvNV) (GLenum target, GLuint index, GLsizei num, const GLfloat* params);
extern void ( * glRequestResidentProgramsNV) (GLsizei n, GLuint* ids);
extern void ( * glTrackMatrixNV) (GLenum target, GLuint address, GLenum matrix, GLenum transform);
extern void ( * glVertexAttrib1dNV) (GLuint index, GLdouble x);
extern void ( * glVertexAttrib1dvNV) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttrib1fNV) (GLuint index, GLfloat x);
extern void ( * glVertexAttrib1fvNV) (GLuint index, const GLfloat* v);
extern void ( * glVertexAttrib1sNV) (GLuint index, GLshort x);
extern void ( * glVertexAttrib1svNV) (GLuint index, const GLshort* v);
extern void ( * glVertexAttrib2dNV) (GLuint index, GLdouble x, GLdouble y);
extern void ( * glVertexAttrib2dvNV) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttrib2fNV) (GLuint index, GLfloat x, GLfloat y);
extern void ( * glVertexAttrib2fvNV) (GLuint index, const GLfloat* v);
extern void ( * glVertexAttrib2sNV) (GLuint index, GLshort x, GLshort y);
extern void ( * glVertexAttrib2svNV) (GLuint index, const GLshort* v);
extern void ( * glVertexAttrib3dNV) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void ( * glVertexAttrib3dvNV) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttrib3fNV) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void ( * glVertexAttrib3fvNV) (GLuint index, const GLfloat* v);
extern void ( * glVertexAttrib3sNV) (GLuint index, GLshort x, GLshort y, GLshort z);
extern void ( * glVertexAttrib3svNV) (GLuint index, const GLshort* v);
extern void ( * glVertexAttrib4dNV) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void ( * glVertexAttrib4dvNV) (GLuint index, const GLdouble* v);
extern void ( * glVertexAttrib4fNV) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void ( * glVertexAttrib4fvNV) (GLuint index, const GLfloat* v);
extern void ( * glVertexAttrib4sNV) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void ( * glVertexAttrib4svNV) (GLuint index, const GLshort* v);
extern void ( * glVertexAttrib4ubNV) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void ( * glVertexAttrib4ubvNV) (GLuint index, const GLubyte* v);
extern void ( * glVertexAttribPointerNV) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
extern void ( * glVertexAttribs1dvNV) (GLuint index, GLsizei n, const GLdouble* v);
extern void ( * glVertexAttribs1fvNV) (GLuint index, GLsizei n, const GLfloat* v);
extern void ( * glVertexAttribs1svNV) (GLuint index, GLsizei n, const GLshort* v);
extern void ( * glVertexAttribs2dvNV) (GLuint index, GLsizei n, const GLdouble* v);
extern void ( * glVertexAttribs2fvNV) (GLuint index, GLsizei n, const GLfloat* v);
extern void ( * glVertexAttribs2svNV) (GLuint index, GLsizei n, const GLshort* v);
extern void ( * glVertexAttribs3dvNV) (GLuint index, GLsizei n, const GLdouble* v);
extern void ( * glVertexAttribs3fvNV) (GLuint index, GLsizei n, const GLfloat* v);
extern void ( * glVertexAttribs3svNV) (GLuint index, GLsizei n, const GLshort* v);
extern void ( * glVertexAttribs4dvNV) (GLuint index, GLsizei n, const GLdouble* v);
extern void ( * glVertexAttribs4fvNV) (GLuint index, GLsizei n, const GLfloat* v);
extern void ( * glVertexAttribs4svNV) (GLuint index, GLsizei n, const GLshort* v);
extern void ( * glVertexAttribs4ubvNV) (GLuint index, GLsizei n, const GLubyte* v);
extern GLenum glewInit ();
extern GLboolean glewIsSupported (const char* name);

extern GLboolean __GLEW_VERSION_1_1;
extern GLboolean __GLEW_VERSION_1_2;
extern GLboolean __GLEW_VERSION_1_2_1;
extern GLboolean __GLEW_VERSION_1_3;
extern GLboolean __GLEW_VERSION_1_4;
extern GLboolean __GLEW_VERSION_1_5;
extern GLboolean __GLEW_VERSION_2_0;
extern GLboolean __GLEW_VERSION_2_1;
extern GLboolean __GLEW_VERSION_3_0;
extern GLboolean __GLEW_VERSION_3_1;
extern GLboolean __GLEW_VERSION_3_2;
extern GLboolean __GLEW_VERSION_3_3;
extern GLboolean __GLEW_VERSION_4_0;
extern GLboolean __GLEW_VERSION_4_1;
extern GLboolean __GLEW_NV_blend_square;
extern GLboolean __GLEW_NV_conditional_render;
extern GLboolean __GLEW_NV_copy_depth_to_color;
extern GLboolean __GLEW_NV_copy_image;
extern GLboolean __GLEW_NV_depth_buffer_float;
extern GLboolean __GLEW_NV_depth_clamp;
extern GLboolean __GLEW_NV_depth_range_unclamped;
extern GLboolean __GLEW_NV_evaluators;
extern GLboolean __GLEW_NV_explicit_multisample;
extern GLboolean __GLEW_NV_fence;
extern GLboolean __GLEW_NV_float_buffer;
extern GLboolean __GLEW_NV_fog_distance;
extern GLboolean __GLEW_NV_fragment_program;
extern GLboolean __GLEW_NV_fragment_program2;
extern GLboolean __GLEW_NV_fragment_program4;
extern GLboolean __GLEW_NV_fragment_program_option;
extern GLboolean __GLEW_NV_framebuffer_multisample_coverage;
extern GLboolean __GLEW_NV_geometry_program4;
extern GLboolean __GLEW_NV_geometry_shader4;
extern GLboolean __GLEW_NV_gpu_program4;
extern GLboolean __GLEW_NV_gpu_program5;
extern GLboolean __GLEW_NV_gpu_program_fp64;
extern GLboolean __GLEW_NV_gpu_shader5;
extern GLboolean __GLEW_NV_half_float;
extern GLboolean __GLEW_NV_light_max_exponent;
extern GLboolean __GLEW_NV_multisample_coverage;
extern GLboolean __GLEW_NV_multisample_filter_hint;
extern GLboolean __GLEW_NV_occlusion_query;
extern GLboolean __GLEW_NV_packed_depth_stencil;
extern GLboolean __GLEW_NV_parameter_buffer_object;
extern GLboolean __GLEW_NV_parameter_buffer_object2;
extern GLboolean __GLEW_NV_pixel_data_range;
extern GLboolean __GLEW_NV_point_sprite;
extern GLboolean __GLEW_NV_present_video;
extern GLboolean __GLEW_NV_primitive_restart;
extern GLboolean __GLEW_NV_register_combiners;
extern GLboolean __GLEW_NV_register_combiners2;
extern GLboolean __GLEW_NV_shader_buffer_load;
extern GLboolean __GLEW_NV_tessellation_program5;
extern GLboolean __GLEW_NV_texgen_emboss;
extern GLboolean __GLEW_NV_texgen_reflection;
extern GLboolean __GLEW_NV_texture_barrier;
extern GLboolean __GLEW_NV_texture_compression_vtc;
extern GLboolean __GLEW_NV_texture_env_combine4;
extern GLboolean __GLEW_NV_texture_expand_normal;
extern GLboolean __GLEW_NV_texture_rectangle;
extern GLboolean __GLEW_NV_texture_shader;
extern GLboolean __GLEW_NV_texture_shader2;
extern GLboolean __GLEW_NV_texture_shader3;
extern GLboolean __GLEW_NV_transform_feedback;
extern GLboolean __GLEW_NV_transform_feedback2;
extern GLboolean __GLEW_NV_vdpau_interop;
extern GLboolean __GLEW_NV_vertex_array_range;
extern GLboolean __GLEW_NV_vertex_array_range2;
extern GLboolean __GLEW_NV_vertex_attrib_integer_64bit;
extern GLboolean __GLEW_NV_vertex_buffer_unified_memory;
extern GLboolean __GLEW_NV_vertex_program;
extern GLboolean __GLEW_NV_vertex_program1_1;
extern GLboolean __GLEW_NV_vertex_program2;
extern GLboolean __GLEW_NV_vertex_program2_option;
extern GLboolean __GLEW_NV_vertex_program3;
extern GLboolean __GLEW_NV_vertex_program4;
]]

local glew = ffi.load('glewgl')
glew.glewInit()

return glew
