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


booln lxEndian_isBig();

short lxEndian_shortBig(short val);
short lxEndian_shortLittle(short  val);
long  lxEndian_longBig(long val);
long  lxEndian_longLittle(long  val);
float lxEndian_floatBig(float val);
float lxEndian_floatLittle(float  val);

#ifdef __cplusplus
};  
#endif

#endif
