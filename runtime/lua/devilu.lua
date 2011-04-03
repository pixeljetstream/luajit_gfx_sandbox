local ffi = require( "ffi" )
ffi.cdef[[
// devilu ilu | DevIL Image Library Utilities

enum {
  ILU_VERSION_1_7_8 = 1,
  ILU_VERSION       = 178
  
, ILU_FILTER         = 0x2600
, ILU_NEAREST        = 0x2601
, ILU_LINEAR         = 0x2602
, ILU_BILINEAR       = 0x2603
, ILU_SCALE_BOX      = 0x2604
, ILU_SCALE_TRIANGLE = 0x2605
, ILU_SCALE_BELL     = 0x2606
, ILU_SCALE_BSPLINE  = 0x2607
, ILU_SCALE_LANCZOS3 = 0x2608
, ILU_SCALE_MITCHELL = 0x2609


// Error types
, ILU_INVALID_ENUM      = 0x0501
, ILU_OUT_OF_MEMORY     = 0x0502
, ILU_INTERNAL_ERROR    = 0x0504
, ILU_INVALID_VALUE     = 0x0505
, ILU_ILLEGAL_OPERATION = 0x0506
, ILU_INVALID_PARAM     = 0x0509


// Values
, ILU_PLACEMENT          = 0x0700
, ILU_LOWER_LEFT         = 0x0701
, ILU_LOWER_RIGHT        = 0x0702
, ILU_UPPER_LEFT         = 0x0703
, ILU_UPPER_RIGHT        = 0x0704
, ILU_CENTER             = 0x0705
, ILU_CONVOLUTION_MATRIX = 0x0710
  
// Languages
, ILU_ENGLISH            = 0x0800
, ILU_ARABIC             = 0x0801
, ILU_DUTCH              = 0x0802
, ILU_JAPANESE           = 0x0803
, ILU_SPANISH            = 0x0804
, ILU_GERMAN             = 0x0805
, ILU_FRENCH             = 0x0806
};


typedef struct ILinfo
{
	ILuint  Id;         // the image's id
	ILubyte *Data;      // the image's data
	ILuint  Width;      // the image's width
	ILuint  Height;     // the image's height
	ILuint  Depth;      // the image's depth
	ILubyte Bpp;        // bytes per pixel (not bits) of the image
	ILuint  SizeOfData; // the total size of the data (in bytes)
	ILenum  Format;     // image format (in IL enum style)
	ILenum  Type;       // image type (in IL enum style)
	ILenum  Origin;     // origin of the image
	ILubyte *Palette;   // the image's palette
	ILenum  PalType;    // palette type
	ILuint  PalSize;    // palette size
	ILenum  CubeFlags;  // flags for what cube map sides are present
	ILuint  NumNext;    // number of images following
	ILuint  NumMips;    // number of mipmaps
	ILuint  NumLayers;  // number of layers
} ILinfo;


typedef struct ILpointf {
	ILfloat x;
	ILfloat y;
} ILpointf;

typedef struct ILpointi {
	ILint x;
	ILint y;
} ILpointi;

 ILboolean      ENTRY iluAlienify();
 ILboolean      ENTRY iluBlurAvg(ILuint Iter);
 ILboolean      ENTRY iluBlurGaussian(ILuint Iter);
 ILboolean      ENTRY iluBuildMipmaps();
 ILuint         ENTRY iluColoursUsed();
 ILboolean      ENTRY iluCompareImage(ILuint Comp);
 ILboolean      ENTRY iluContrast(ILfloat Contrast);
 ILboolean      ENTRY iluCrop(ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth);
 ILboolean      ENTRY iluEdgeDetectE();
 ILboolean      ENTRY iluEdgeDetectP();
 ILboolean      ENTRY iluEdgeDetectS();
 ILboolean      ENTRY iluEmboss();
 ILboolean      ENTRY iluEnlargeCanvas(ILuint Width, ILuint Height, ILuint Depth);
 ILboolean      ENTRY iluEnlargeImage(ILfloat XDim, ILfloat YDim, ILfloat ZDim);
 ILboolean      ENTRY iluEqualize();
 ILconst_string 		 ENTRY iluErrorString(ILenum Error);
 ILboolean      ENTRY iluConvolution(ILint *matrix, ILint scale, ILint bias);
 ILboolean      ENTRY iluFlipImage();
 ILboolean      ENTRY iluGammaCorrect(ILfloat Gamma);
 void           ENTRY iluGetImageInfo(ILinfo *Info);
 ILint          ENTRY iluGetInteger(ILenum Mode);
 void           ENTRY iluGetIntegerv(ILenum Mode, ILint *Param);
 ILstring 		 ENTRY iluGetString(ILenum StringName);
 void           ENTRY iluImageParameter(ILenum PName, ILenum Param);
 void           ENTRY iluInit();
 ILboolean      ENTRY iluInvertAlpha();
 ILuint         ENTRY iluLoadImage(ILconst_string FileName);
 ILboolean      ENTRY iluMirror();
 ILboolean      ENTRY iluNegative();
 ILboolean      ENTRY iluNoisify(ILclampf Tolerance);
 ILboolean      ENTRY iluPixelize(ILuint PixSize);
 void           ENTRY iluRegionfv(ILpointf *Points, ILuint n);
 void           ENTRY iluRegioniv(ILpointi *Points, ILuint n);
 ILboolean      ENTRY iluReplaceColour(ILubyte Red, ILubyte Green, ILubyte Blue, ILfloat Tolerance);
 ILboolean      ENTRY iluRotate(ILfloat Angle);
 ILboolean      ENTRY iluRotate3D(ILfloat x, ILfloat y, ILfloat z, ILfloat Angle);
 ILboolean      ENTRY iluSaturate1f(ILfloat Saturation);
 ILboolean      ENTRY iluSaturate4f(ILfloat r, ILfloat g, ILfloat b, ILfloat Saturation);
 ILboolean      ENTRY iluScale(ILuint Width, ILuint Height, ILuint Depth);
 ILboolean      ENTRY iluScaleAlpha(ILfloat scale);
 ILboolean      ENTRY iluScaleColours(ILfloat r, ILfloat g, ILfloat b);
 ILboolean      ENTRY iluSetLanguage(ILenum Language);
 ILboolean      ENTRY iluSharpen(ILfloat Factor, ILuint Iter);
 ILboolean      ENTRY iluSwapColours();
 ILboolean      ENTRY iluWave(ILfloat Angle);

]]

return ffi.load( "ILU" )