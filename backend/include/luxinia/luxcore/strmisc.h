// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_STRMISC_H__
#define __LUXCORE_STRMISC_H__

#include <luxinia/luxplatform/luxplatform.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

#define LUX_STR_FORMAT_VECTOR3 "(%f,%f,%f)"
#define LUX_STR_FORMAT_VECTOR4 "(%f,%f,%f,%f)"

//////////////////////////////////////////////////////////////////////////
// string helpers

LUX_API void lxStrPrintf(char *buffer, size_t buffersize, const char *fmt, ...);
LUX_API int  lxStrBeginsWith(const char *check, const char *search);
LUX_API int  lxStrSplitList( char *liststart,  char **items, const char sep,int maxitems);

  // replace string must be shorter or equal search string, tail char is used to pad shoter ones
LUX_API int lxStrReplace(char *str,const char *search,const char *replace,const char trail);

  // output must be same size as input
  // output will always be a valid string
LUX_API char* lxStrToLower(char* output, const char *input);

  // output must be same size as input
  // output will always be a valid string
LUX_API char* lxStrToUpper(char* output, const char *input);

  // out must be at least filename size
LUX_API char* lxStrGetPath(char *out,const char *filename);

  // returns formatted integer eg. 1,000 
LUX_API char* lxStrFormatedNumber(char *buffer,size_t buffersize,int number,int sep);

LUX_API const char* lxStrFindAsLower(const char* inputaslower, const char *search);

  // converts bytes to char string (buffer.size >= len*2, returns len converted)
LUX_API size_t lxStrHexFromByte(char *buffer, size_t bufferlen, const byte *databytes, size_t len);
  // convertes char string to bytes
LUX_API size_t lxStrByteFromHex(byte *buffer, size_t bufferlen, const char *datahex);

  // returns end of quote in inStr
LUX_API const char* lxStrReadInQuotes(const char *inStr, char *outStr, size_t outSize);
LUX_API int   lxStrReadBitMask(const char* str, enum32 *val);
LUX_API int   lxStrReadVector3(const char* str, float vec[3]);
LUX_API int   lxStrReadVector4(const char* str, float vec[4]);
LUX_API int   lxStrReadFloat(const char* str, float *val);
LUX_API int   lxStrReadInt(const char* str, int *val);

LUX_API booln lxStrHasPath(const char *str);

LUX_API uint32  lxStrHash(const char *str);
LUX_API uint32  lxStrHashSimple(const char *key);
LUX_API uint32  lxStrHashCrc32(const void *data, size_t size);
LUX_API uint32  lxStrMurmurHash2( const void * key, size_t len, uint32 seed );
LUX_API uint32  lxStrMurmurHash2A ( const void * key, size_t len, uint32 seed );

#ifdef __cplusplus
}
#endif

#endif

