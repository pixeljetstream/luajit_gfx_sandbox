local ffi = require 'ffi'
ffi.cdef [[
// assimp ai | AssetImporter Model Loader Library
typedef enum aiBool {
  aiBool_FALSE = 0,
  aiBool_TRUE = 1,
} aiBool;
typedef enum aiReturn {
  aiReturn_SUCCESS = 0x0,
  aiReturn_FAILURE = -0x1,
  aiReturn_OUTOFMEMORY = -0x3,
} aiReturn;
typedef enum aiOrigin {
  aiOrigin_SET = 0x0,
  aiOrigin_CUR = 0x1,
  aiOrigin_END = 0x2,
} aiOrigin;
typedef enum aiDefaultLogStream {
  aiDefaultLogStream_FILE = 0x1,
  aiDefaultLogStream_STDOUT = 0x2,
  aiDefaultLogStream_STDERR = 0x4,
  aiDefaultLogStream_DEBUGGER = 0x8,
} aiDefaultLogStream;
typedef enum aiComponent {
  aiComponent_NORMALS = 0x2,
  aiComponent_TANGENTS_AND_BITANGENTS = 0x4,
  aiComponent_COLORS = 0x8,
  aiComponent_TEXCOORDS = 0x10,
  aiComponent_BONEWEIGHTS = 0x20,
  aiComponent_ANIMATIONS = 0x40,
  aiComponent_TEXTURES = 0x80,
  aiComponent_LIGHTS = 0x100,
  aiComponent_CAMERAS = 0x200,
  aiComponent_MESHES = 0x400,
  aiComponent_MATERIALS = 0x800,
} aiComponent;
typedef enum aiLightSourceType {
  aiLightSourceType_UNDEFINED = 0x0,
  aiLightSourceType_DIRECTIONAL = 0x1,
  aiLightSourceType_POINT = 0x2,
  aiLightSourceType_SPOT = 0x3,
} aiLightSourceType;
typedef enum aiAnimBehaviour {
  aiAnimBehaviour_DEFAULT = 0x0,
  aiAnimBehaviour_CONSTANT = 0x1,
  aiAnimBehaviour_LINEAR = 0x2,
  aiAnimBehaviour_REPEAT = 0x3,
} aiAnimBehaviour;
typedef enum aiPrimitiveType {
  aiPrimitiveType_POINT = 0x1,
  aiPrimitiveType_LINE = 0x2,
  aiPrimitiveType_TRIANGLE = 0x4,
  aiPrimitiveType_POLYGON = 0x8,
} aiPrimitiveType;
typedef enum aiTextureOp {
  aiTextureOp_Multiply = 0x0,
  aiTextureOp_Add = 0x1,
  aiTextureOp_Subtract = 0x2,
  aiTextureOp_Divide = 0x3,
  aiTextureOp_SmoothAdd = 0x4,
  aiTextureOp_SignedAdd = 0x5,
} aiTextureOp;
typedef enum aiTextureMapMode {
  aiTextureMapMode_Wrap = 0x0,
  aiTextureMapMode_Clamp = 0x1,
  aiTextureMapMode_Decal = 0x3,
  aiTextureMapMode_Mirror = 0x2,
} aiTextureMapMode;
typedef enum aiTextureMapping {
  aiTextureMapping_UV = 0x0,
  aiTextureMapping_SPHERE = 0x1,
  aiTextureMapping_CYLINDER = 0x2,
  aiTextureMapping_BOX = 0x3,
  aiTextureMapping_PLANE = 0x4,
  aiTextureMapping_OTHER = 0x5,
} aiTextureMapping;
typedef enum aiTextureType {
  aiTextureType_NONE = 0x0,
  aiTextureType_DIFFUSE = 0x1,
  aiTextureType_SPECULAR = 0x2,
  aiTextureType_AMBIENT = 0x3,
  aiTextureType_EMISSIVE = 0x4,
  aiTextureType_HEIGHT = 0x5,
  aiTextureType_NORMALS = 0x6,
  aiTextureType_SHININESS = 0x7,
  aiTextureType_OPACITY = 0x8,
  aiTextureType_DISPLACEMENT = 0x9,
  aiTextureType_LIGHTMAP = 0xA,
  aiTextureType_REFLECTION = 0xB,
  aiTextureType_UNKNOWN = 0xC,
} aiTextureType;
typedef enum aiShadingMode {
  aiShadingMode_Flat = 0x1,
  aiShadingMode_Gouraud = 0x2,
  aiShadingMode_Phong = 0x3,
  aiShadingMode_Blinn = 0x4,
  aiShadingMode_Toon = 0x5,
  aiShadingMode_OrenNayar = 0x6,
  aiShadingMode_Minnaert = 0x7,
  aiShadingMode_CookTorrance = 0x8,
  aiShadingMode_NoShading = 0x9,
  aiShadingMode_Fresnel = 0xa,
} aiShadingMode;
typedef enum aiTextureFlags {
  aiTextureFlags_Invert = 0x1,
  aiTextureFlags_UseAlpha = 0x2,
  aiTextureFlags_IgnoreAlpha = 0x4,
} aiTextureFlags;
typedef enum aiBlendMode {
  aiBlendMode_Default = 0x0,
  aiBlendMode_Additive = 0x1,
} aiBlendMode;
typedef enum aiPropertyTypeInfo {
  aiPTI_Float = 0x1,
  aiPTI_String = 0x3,
  aiPTI_Integer = 0x4,
  aiPTI_Buffer = 0x5,
} aiPropertyTypeInfo;
typedef enum aiSceneFlags {
  aiSceneFlags_INCOMPLETE = 0x1,
  aiSceneFlags_VALIDATED = 0x2,
  aiSceneFlags_VALIDATION_WARNING = 0x4,
  aiSceneFlags_NON_VERBOSE_FORMAT = 0x8,
  aiSceneFlags_FLAGS_TERRAIN = 0x10,
} aiSceneFlags;
typedef enum aiPostProcessSteps {
  aiProcess_CalcTangentSpace = 0x1,
  aiProcess_JoinIdenticalVertices = 0x2,
  aiProcess_MakeLeftHanded = 0x4,
  aiProcess_Triangulate = 0x8,
  aiProcess_RemoveComponent = 0x10,
  aiProcess_GenNormals = 0x20,
  aiProcess_GenSmoothNormals = 0x40,
  aiProcess_SplitLargeMeshes = 0x80,
  aiProcess_PreTransformVertices = 0x100,
  aiProcess_LimitBoneWeights = 0x200,
  aiProcess_ValidateDataStructure = 0x400,
  aiProcess_ImproveCacheLocality = 0x800,
  aiProcess_RemoveRedundantMaterials = 0x1000,
  aiProcess_FixInfacingNormals = 0x2000,
  aiProcess_SortByPType = 0x8000,
  aiProcess_FindDegenerates = 0x10000,
  aiProcess_FindInvalidData = 0x20000,
  aiProcess_GenUVCoords = 0x40000,
  aiProcess_TransformUVCoords = 0x80000,
  aiProcess_FindInstances = 0x100000,
  aiProcess_OptimizeMeshes = 0x200000,
  aiProcess_OptimizeGraph = 0x400000,
  aiProcess_FlipUVs = 0x800000,
  aiProcess_FlipWindingOrder = 0x1000000,
} aiPostProcessSteps;
typedef unsigned int uint;
typedef char byte;
typedef unsigned char ubyte;
typedef void (*aiLogStreamCallback)( char* message, char* user ) ;
typedef size_t (*aiFileWriteProc)( struct aiFile*, char*, size_t, size_t ) ;
typedef size_t (*aiFileReadProc)(  struct aiFile*, char*, size_t, size_t ) ;
typedef size_t (*aiFileTellProc)( struct aiFile* ) ;
typedef void (*aiFileFlushProc)( struct aiFile* ) ;
typedef aiReturn (*aiFileSeek)( struct aiFile*, size_t, aiOrigin ) ;
typedef struct aiFile* (*aiFileOpenProc)( struct aiFileIO*, char*, char* ) ;
typedef void (*aiFileCloseProc)( struct aiFileIO*,  struct aiFile* ) ;
typedef char* aiUserData;
static const uint ASSIMP_CFLAGS_SHARED = 0x1;
static const uint ASSIMP_CFLAGS_STLPORT = 0x2;
static const uint ASSIMP_CFLAGS_DEBUG = 0x4;
static const uint ASSIMP_CFLAGS_NOBOOST = 0x8;
static const uint ASSIMP_CFLAGS_SINGLETHREADED = 0x10;
static const size_t AI_TYPES_MAXLEN = 1024;
static const uint AI_SLM_DEFAULT_MAX_TRIANGLES = 1000000;
static const uint AI_SLM_DEFAULT_MAX_VERTICES = 1000000;
static const uint AI_LMW_MAX_WEIGHTS = 0x4;
static const uint AI_UVTRAFO_SCALING = 0x1;
static const uint AI_UVTRAFO_ROTATION = 0x2;
static const uint AI_UVTRAFO_TRANSLATION = 0x4;
static const uint AI_MAX_FACE_INDICES = 0x7fff;
static const uint AI_MAX_BONE_WEIGHTS = 0x7fffffff;
static const uint AI_MAX_VERTICES = 0x7fffffff;
static const uint AI_MAX_FACES = 0x7fffffff;
static const uint AI_MAX_NUMBER_OF_COLOR_SETS = 0x4;
static const uint AI_MAX_NUMBER_OF_TEXTURECOORDS = 0x4;
typedef struct aiLogStream {
  aiLogStreamCallback callback;
  char* user;
} aiLogStream;
typedef struct aiString {
  size_t length;
  char data[1024];
} aiString;
typedef struct aiMemoryInfo {
  uint textures;
  uint materials;
  uint meshes;
  uint nodes;
  uint animations;
  uint cameras;
  uint lights;
  uint total;
} aiMemoryInfo;
typedef struct aiVector2D {
  float x, y;
} aiVector2D;
typedef struct aiVector3D {
  float x, y, z;
} aiVector3D;
typedef struct aiQuaternion {
  float w, x, y, z;
} aiQuaternion;
typedef struct aiMatrix3x3 {
  float a1, a2, a3;
  float b1, b2, b3;
  float c1, c2, c3;
} aiMatrix3x3;
typedef struct aiMatrix4x4 {
  float a1, a2, a3, a4;
  float b1, b2, b3, b4;
  float c1, c2, c3, c4;
  float d1, d2, d3, d4;
} aiMatrix4x4;
typedef struct aiPlane {
  float a;
  float b;
  float c;
  float d;
} aiPlane;
typedef struct aiRay {
  aiVector3D pos;
  aiVector3D dir;
} aiRay;
typedef struct aiColor3D {
  float r;
  float g;
  float b;
} aiColor3D;
typedef struct aiColor4D {
  float r;
  float g;
  float b;
  float a;
} aiColor4D;
typedef struct aiFileIO {
  aiFileOpenProc OpenProc;
  aiFileCloseProc CloseProc;
  aiUserData UserData;
} aiFileIO;
typedef struct aiFile {
  aiFileReadProc ReadProc;
  aiFileWriteProc WriteProc;
  aiFileTellProc TellProc;
  aiFileTellProc FileSizeProc;
  aiFileSeek SeekProc;
  aiFileFlushProc FlushProc;
  aiUserData UserData;
} aiFile;
typedef struct aiLight {
  aiString mName;
  aiLightSourceType mType;
  aiVector3D mPosition;
  aiVector3D mDirection;
  float mAttenuationConstant;
  float mAttenuationLinear;
  float mAttenuationQuadratic;
  aiColor3D mColorDiffuse;
  aiColor3D mColorSpecular;
  aiColor3D mColorAmbient;
  float mAngleInnerCone;
  float mAngleOuterCone;
} aiLight;
typedef struct aiCamera {
  aiString mName;
  aiVector3D mPosition;
  aiVector3D mUp;
  aiVector3D mLookAt;
  float mHorizontalFOV;
  float mClipPlaneNear;
  float mClipPlaneFar;
  float mAspect;
} aiCamera;
typedef struct aiVectorKey {
  double mTime;
  aiVector3D mValue;
} aiVectorKey;
typedef struct aiQuatKey {
  double mTime;
  aiQuaternion mValue;
} aiQuatKey;
typedef struct aiNodeAnim {
  aiString mNodeName;
  uint mNumPositionKeys;
  aiVectorKey* mPositionKeys;
  uint mNumRotationKeys;
  aiQuatKey* mRotationKeys;
  uint mNumScalingKeys;
  aiVectorKey* mScalingKeys;
  aiAnimBehaviour mPreState;
  aiAnimBehaviour mPostState;
} aiNodeAnim;
typedef struct aiAnimation {
  aiString mName;
  double mDuration;
  double mTicksPerSecond;
  uint mNumChannels;
  aiNodeAnim** mChannels;
} aiAnimation;
typedef struct aiFace {
  uint mNumIndices;
  uint* mIndices;
} aiFace;
typedef struct aiVertexWeight {
  uint mVertexId;
  float mWeight;
} aiVertexWeight;
typedef struct aiBone {
  aiString mName;
  uint mNumWeights;
  aiVertexWeight* mWeights;
  aiMatrix4x4 mOffsetMatrix;
} aiBone;
typedef struct aiAnimMesh {
  aiVector3D* mVertices;
  aiVector3D* mNormals;
  aiVector3D* mTangents;
  aiVector3D* mBitangents;
  aiColor4D* mColors[0x4];
  aiVector3D* mTextureCoords[0x4];
  uint mNumVertices;
} aiAnimMesh;
typedef struct aiMesh {
  uint mPrimitiveTypes;
  uint mNumVertices;
  uint mNumFaces;
  aiVector3D* mVertices;
  aiVector3D* mNormals;
  aiVector3D* mTangents;
  aiVector3D* mBitangents;
  aiColor4D* mColors[0x4];
  aiVector3D* mTextureCoords[0x4];
  uint mNumUVComponents[0x4];
  aiFace* mFaces;
  uint mNumBones;
  aiBone** mBones;
  uint mMaterialIndex;
  aiString mName;
  uint mNumAnimMeshes;
  aiAnimMesh** mAnimMeshes;
} aiMesh;
typedef struct aiUVTransform {
  aiVector2D mTranslation;
  aiVector2D mScaling;
  float mRotation;
} aiUVTransform;
typedef struct aiMaterialProperty {
  aiString mKey;
  uint mSemantic;
  uint mIndex;
  uint mDataLength;
  aiPropertyTypeInfo mType;
  char* mData;
} aiMaterialProperty;
typedef struct aiMaterial {
  aiMaterialProperty** mProperties;
  uint mNumProperties;
  uint mNumAllocated;
} aiMaterial;
typedef struct aiTexel {
  ubyte b, g, r, a;
} aiTexel;
typedef struct aiTexture {
  uint mWidth;
  uint mHeight;
  char achFormatHint[4];
  aiTexel* pcData;
} aiTexture;
typedef struct aiNode {
  aiString mName;
  aiMatrix4x4 mTransformation;
  struct aiNode* mParent;
  uint mNumChildren;
  struct aiNode** mChildren;
  int mNumMeshes;
  uint* mMeshes;
} aiNode;
typedef struct aiScene {
  uint mFlags;
  aiNode* mRootNode;
  uint mNumMeshes;
  aiMesh** mMeshes;
  uint mNumMaterials;
  aiMaterial** mMaterials;
  uint mNumAnimations;
  aiAnimation** mAnimations;
  uint mNumTextures;
  aiTexture** mTextures;
  uint mNumLights;
  aiLight** mLights;
  uint mNumCameras;
  aiCamera** mCameras;
} aiScene;
aiScene* aiImportFile( const char* pFile, uint pFile );
aiScene* aiImportFileEx( const char* pFile, uint pFlags, aiFileIO* pFS );
aiScene* aiImportFileFromMemory( const char* pBuffer, uint pLength, uint pFlags, const char* pHint );
aiScene* aiApplyPostProcessing( aiScene* pScene, uint pFlags );
aiLogStream aiGetPredefinedLogStream( aiDefaultLogStream pStreams, const char* file );
void aiAttachLogStream( aiLogStream* stream );
void aiEnableVerboseLogging( aiBool d );
aiReturn aiDetachLogStream( aiLogStream* stream );
void aiDetachAllLogStreams();
void aiReleaseImport( aiScene* pScene );
char* aiGetErrorString();
aiBool aiIsExtensionSupported( const char* szExtension );
void aiGetExtensionList( aiString* szOut );
void aiGetMemoryRequirements( aiScene* pIn, aiMemoryInfo* info );
void aiSetImportPropertyInteger( const char* szName, int value );
void aiSetImportPropertyFloat( const char* szName, float value );
void aiSetImportPropertyString( const char* szName, aiString* st );
void aiCreateQuaternionFromMatrix( aiQuaternion* quat, aiMatrix3x3* mat );
void aiDecomposeMatrix( aiMatrix4x4* mat, aiVector3D* scaling, aiQuaternion* rotation, aiVector3D* position );
void aiTransposeMatrix4( aiMatrix4x4* mat );
void aiTransposeMatrix3( aiMatrix3x3* mat );
void aiTransformVecByMatrix3( aiVector3D* vec, aiMatrix3x3* mat );
void aiTransformVecByMatrix4( aiVector3D* vec, aiMatrix4x4* mat );
void aiMultiplyMatrix4( aiMatrix4x4* dst, aiMatrix4x4* src );
void aiMultiplyMatrix3( aiMatrix3x3* dst, aiMatrix3x3* src );
void aiIdentityMatrix3( aiMatrix3x3* mat );
void aiIdentityMatrix4( aiMatrix4x4* mat );
aiReturn aiGetMaterialProperty( aiMaterial* pMat, const char* pKey, uint type, uint index, aiMaterialProperty** pPropOut );
aiReturn aiGetMaterialFloatArray( aiMaterial* pMat, const char* pKey, uint type, uint index, float* pOut, uint* pMax );
aiReturn aiGetMaterialIntegerArray( aiMaterial* pMat, const char* pKey, uint type, uint index, int* pOut, uint* pMax );
aiReturn aiGetMaterialColor( aiMaterial* pMat, const char* pKey, uint type, uint index, aiColor4D* pOut );
aiReturn aiGetMaterialString( aiMaterial* pMat, const char* pKey, uint type, uint index, aiString* pOut );
uint aiGetMaterialTextureCount( aiMaterial* pMat, aiTextureType type );
aiReturn aiGetMaterialTexture( aiMaterial* mat, aiTextureType type, uint index, aiString* path, aiTextureMapping* mapping , uint* uvindex , float* blend , aiTextureOp* op , aiTextureMapMode* mapmode );
char* aiGetLegalString();
uint aiGetVersionMinor();
uint aiGetVersionMajor();
uint aiGetVersionRevision();
uint aiGetCompileFlags();
]]

return ffi.load('assimp')

