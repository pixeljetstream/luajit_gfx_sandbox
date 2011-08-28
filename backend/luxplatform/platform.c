// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxplatform/debug.h>
#include <stdarg.h>

#ifdef LUX_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <signal.h>

//////////////////////////////////////////////////////////////////////////
// Debug

LUX_API void lxDebugAssertFailed( const char *file, int line, const char *expression )
{
  lxDebugPrintf("Assert Failed: %s,%d; expr: %s\n",file,line,expression);

#if defined(LUX_PLATFORM_WINDOWS)
  #if defined(LUX_ARCH_X86)
    #if defined(LUX_COMPILER_GCC)
      __asm__ __volatile__ ("int $0x03");
    #elif defined(LUX_COMPILER_MSC)
      __asm int 0x03;
    #endif
  #elif defined(LUX_ARCH_X64)
    raise(SIGINT);
  #endif

#elif defined(LUX_PLATFORM_MAC)
  kill( getpid(), SIGINT );

#elif defined(LUX_PLATFORM_LINUX)
  #if defined(LUX_ARCH_X86)
    __asm__ __volatile__ ("int $0x03");
  #else
    raise(SIGINT);
  #endif
#endif

}

LUX_API void lxDebugPrintf(const char* fmt, ...)
{
  // FIXME
  char    text[1024];
  size_t    size = sizeof(text);
  va_list   ap;

  if (fmt == NULL) return;
  va_start(ap, fmt);
#ifdef LUX_COMPILER_MSC
    _vsnprintf(text, size, fmt, ap);
#elif defined (LUX_COMPILER_GCC)
    vsnprintf(text, size, fmt, ap);
#else
#error "compiler unsupported"
#endif
  va_end(ap);

  text[size-1] = '\0';

  printf(text);

#ifdef LUX_PLATFORM_WINDOWS
  OutputDebugStringA(text);
#endif

}


//////////////////////////////////////////////////////////////////////////
// Endianess

LUX_API booln lxEndian_isBig()
{
#if defined(LUX_ENDIAN_BIG)
  booln state = LUX_TRUE;
#else
  booln state = LUX_FALSE;
#endif

  // sanity check
#ifdef LUX_DEVBUILD
  byte one[2] = {1,0};
  LUX_ASSERT( *(short*)one != 1 &&  !state);
#endif

  return state;
}

#if defined(LUX_ENDIAN_BIG)

#define PF_shortSwap  lxEndian_shortLittle
#define PF_shortPass  lxEndian_shortBig

#define PF_longSwap   lxEndian_longLittle
#define PF_longPass   lxEndian_longBig

#define PF_floatSwap  lxEndian_floatLittle
#define PF_floatPass  lxEndian_floatBig

#else

#define PF_shortSwap  lxEndian_shortBig
#define PF_shortPass  lxEndian_shortLittle

#define PF_longSwap   lxEndian_longBig
#define PF_longPass   lxEndian_longLittle

#define PF_floatSwap  lxEndian_floatBig
#define PF_floatPass  lxEndian_floatLittle

#endif

typedef union{
  float f;
  short s;
  long  l;
  byte  b[4];
}endconv_t;

LUX_API short PF_shortSwap(short  val){
  endconv_t input;
  endconv_t output;
  input.s = 0;

  output.b[0] = input.b[1];
  output.b[1] = input.b[0];

  return output.s;
}
LUX_API short PF_shortPass(short  val){
  return val;
}
LUX_API long  PF_longSwap(long  val)
{
  endconv_t input;
  endconv_t output;
  input.l = val;

  output.b[0] = input.b[3];
  output.b[1] = input.b[2];
  output.b[2] = input.b[1];
  output.b[3] = input.b[0];
  return output.l;
}
LUX_API long  PF_longPass(long  val)
{
  return val;
}
LUX_API float PF_floatSwap(float  val)
{
  endconv_t input;
  endconv_t output;
  input.f = val;

  output.b[0] = input.b[3];
  output.b[1] = input.b[2];
  output.b[2] = input.b[1];
  output.b[3] = input.b[0];
  return output.f;
}
LUX_API float PF_floatPass(float  val)
{
  return val;
}

#undef PF_shortSwap
#undef PF_shortPass

#undef PF_longSwap
#undef PF_longPass

#undef PF_floatSwap
#undef PF_floatPass