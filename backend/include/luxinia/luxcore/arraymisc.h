// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h


#ifndef __LUXCORE_ARRAYMISC_H__
#define __LUXCORE_ARRAYMISC_H__

#include <luxinia/luxplatform/luxplatform.h>

#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////
// Array operations

	// searches or appends value to list (count must be preset)
	// returns position in list, or
	// -1 if maxcnt is reached prior append
LUX_API int lxArrayFindOrAdd8(uint8 *list, int *inoutCnt, uint8 value, int maxCnt);
LUX_API int lxArrayFindOrAdd16(uint16 *list, int *inoutCnt, uint16 value, int maxCnt);
LUX_API int lxArrayFindOrAdd32(uint32 *list, int *inoutCnt, uint32 value, int maxCnt);
LUX_API int lxArrayFindOrAddFloat(float *list, int *inoutCnt, float value, int maxCnt);
LUX_API int lxArrayFindOrAddPtr(void **list, int *inoutCnt, void *value, int maxCnt);

#ifdef __cplusplus
};  
#endif

#endif
