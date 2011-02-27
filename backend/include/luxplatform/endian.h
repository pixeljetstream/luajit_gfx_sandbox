// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt



#ifndef __LUXPLATFORM_ENDIAN_H__
#define __LUXPLATFORM_ENDIAN_H__

#include <luxplatform/luxplatform.h>

#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////
// Endianess


LUX_API booln lxEndian_isBig();

LUX_API short lxEndian_shortBig(short val);
LUX_API short lxEndian_shortLittle(short  val);
LUX_API long  lxEndian_longBig(long val);
LUX_API long  lxEndian_longLittle(long  val);
LUX_API float lxEndian_floatBig(float val);
LUX_API float lxEndian_floatLittle(float  val);

#ifdef __cplusplus
};  
#endif

#endif
