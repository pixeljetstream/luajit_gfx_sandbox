// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/strmisc.h>
#include <stdarg.h>
#include <ctype.h>

LUX_API void lxStrPrintf(char *buffer, size_t buffersize, const char *fmt, ...)
{
  va_list   ap;

  if (fmt == NULL) return;
  va_start(ap, fmt);
#ifdef LUX_COMPILER_MSC
    _vsnprintf(buffer, buffersize, fmt, ap);
#elif defined (LUX_COMPILER_GCC)
    vsnprintf(buffer, buffersize, fmt, ap);
#else
  #error "compiler unsupported"
#endif
  va_end(ap);

  buffer[buffersize-1] = '\0';
}

LUX_API const char* lxStrReadInQuotes(const char *inStr, char *outStr, size_t outSize)
{
  const char *in = inStr;
  char test;
  char *outEnd = outStr+outSize;
  memset(outStr, 0, outSize);

  while ((test=*in) && test != '\"')in++;
  if (!test)
    return in;
  in++;
  while ((test=*in) && test != '\"' && outStr < outEnd)
  {
    *outStr++ = test;
    in++;
  }
  if (*in == '\"')
    return (++in);
  else
    return NULL;
}

LUX_API int   lxStrReadBitMask(const char* str, enum32 *val){
  int i = 0;
  *val = 0;

  while (str[i] && i < 32){
    if (str[i] == '1') 
      *val |= 1<<i++; 
    else if (str[i] == '0')
      i++;
    else
      return i;
      
  }
  return i;
}
LUX_API int   lxStrReadVector3(const char* str, float vec[3]){
  return sscanf(str,LUX_STR_FORMAT_VECTOR3,&vec[0],&vec[1],&vec[2]);
}
LUX_API int   lxStrReadVector4(const char* str, float vec[4]){
  return sscanf(str,LUX_STR_FORMAT_VECTOR4,&vec[0],&vec[1],&vec[2],&vec[3]);
}
LUX_API int   lxStrReadFloat(const char* str, float *val){
  return sscanf(str,"%f",val);
}
LUX_API int   lxStrReadInt(const char* str, int *val){
  return sscanf(str,"%i",val);
}

LUX_API booln lxStrHasPath(const char *str){
  const char *search = str;
  while (*search != 0){
    if (*search == '/' || *search == '\\'){
      return LUX_TRUE;
    }
    search++;
  }

  return LUX_FALSE;
}

LUX_API int lxStrBeginsWith(const char *check, const char *search)
{
  const char *src = check;
  const char *id = search;

  if (!src || !id)
    return LUX_FALSE;

  while(*src == *id){
    src++;
    id++;
  }

  if (*id )
    return LUX_FALSE;
  else
    return LUX_TRUE;
}

LUX_API int lxStrSplitList( char *letter,  char **items, const char sep,int numtex)
{
  int tex;

  tex = 0;
  while (*letter && tex < numtex){
    items[tex] = letter;
    while (*letter && *letter != sep){
      letter++;
    }
    *letter = 0;
    letter++;
    tex++;
  }
  return (numtex < tex) ? numtex : tex;
}

LUX_API int lxStrReplace(char *str,const char *search,const char *replace,const char trail)
{
  char *curchar;
  const char *repchar;
  int searchlen = strlen(search);
  int replen = strlen(replace);
  int i;
  int cnt = 0;

  while(curchar = strstr(str,search)){
    repchar = replace;
    cnt++;
    for(i = 0; i < searchlen; i++,curchar++,repchar++){
      if (i < replen)
        *curchar = *repchar;
      else
        *curchar = trail;
    }
  }

  return cnt;
}

LUX_API char* lxStrGetPath(char *out,const char *filename)
{
  const char  *browse = filename;
  char *lastslash = NULL;
  char  *pwrite = out;

  out[0] = '\0';

  while (browse && *browse && *browse != '.'){
    if (*browse == '/' || *browse == '\\')
      lastslash = pwrite;
    *pwrite = *browse;
    browse++;
    pwrite++;
  }

  if (lastslash){
    lastslash++;
    *lastslash = 0;
  }

  return out;
}

LUX_API char* lxStrFormatedNumber(char *buffer,size_t buffersize, int number,int sep)
{
  char *ptr;
  switch(sep)
  {
  case 0:
    lxStrPrintf(buffer,buffersize,"%3d",number);
    break;
  case 1:
    if (number > 1000)
      lxStrPrintf(buffer,buffersize,"%3d,%3d",number/1000,number%1000);
    else 
      lxStrPrintf(buffer,buffersize,"%7d",number);
    break;
  case 2:
    if (number > 1000000)
      lxStrPrintf(buffer,buffersize,"%3d,%3d,%3d",number/1000000,(number%1000000)/1000,(number%1000000)%1000);
    else if (number > 1000)
      lxStrPrintf(buffer,buffersize,"%7d,%3d",number/1000,number%1000);
    else 
      lxStrPrintf(buffer,buffersize,"%11d",number);
    break;
  case 3:
    if (number > 1000000000)
      lxStrPrintf(buffer,buffersize,"%3d,%3d,%3d,%3d",number/1000000000,(number%1000000000)/1000000,((number%1000000000)%1000000)/1000,((number%1000000000)%1000000)%1000);
    else if (number > 1000000)
      lxStrPrintf(buffer,buffersize,"%7d,%3d,%3d",number/1000000,(number%1000000)/1000,(number%1000000)%1000);
    else if (number > 1000)
      lxStrPrintf(buffer,buffersize,"%11d,%3d",number/1000,number%1000);
    else 
      lxStrPrintf(buffer,buffersize,"%15d",number);
    break;
  }
  ptr = buffer+1;
  while (*ptr){
    if(*ptr == ' ' && (*(ptr-1) == ',' || *(ptr-1) == '0' ))
      *ptr = '0';
    ptr++;
  }

  return buffer;
}

LUX_API char* lxStrToLower(char* output, const char *input)
{
  const char *c = input;
  char *o = output;

  if (!input || input[0] == 0){
    output[0] = 0;
    return output;
  }

  while (c[0]){
    o[0] = tolower(c[0]);
    o++;
    c++;
  }
  o[0] = 0;

  return output;
}

LUX_API char* lxStrToUpper(char* output, const char *input)
{
  const char *c = input;
  char *o = output;

  if (!input || input[0] == 0){
    output[0] = 0;
    return output;
  }

  while (c[0]){
    o[0] = toupper(c[0]);
    o++;
    c++;
  }
  o[0] = 0;

  return output;
}

LUX_API const char* lxStrFindAsLower(const char* input, const char *search)
{
  const char* is = input;

  while (*is){
    const char* i = is;
    const char* s = search;
    while (*i && tolower(*i) == *s){
      i++;
      s++;
      if (!*s){
        return is;
      }
    }
    is++;
  }
  return NULL;
}

LUX_API size_t lxStrHexFromByte(char *buffer, size_t bufferlen, const byte *data, size_t len)
{
  const char tostr[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  const uchar *d = (const uchar *)data;
  char *out = buffer;
  size_t i = 0;
  for (; i < len && (i*2)+1 < bufferlen; i++,d++,out+=2){
    uint val = *d;
    uint hi = val/16;
    uint lo = val%16;
    out[0] = tostr[hi];
    out[1] = tostr[lo];
  }

  return i*2;
}

#define HEXTOINT(val) ((val) > '9' ? (val)-'A'+10 : (val)-'0')

LUX_API size_t lxStrByteFromHex(byte *buffer, size_t bufferlen, const char *data)
{
  const char *c = data;
  uchar *out = (uchar *)buffer;
  size_t i = 0;

  // must be BYTES
  size_t len = strlen(data);
  if (len % 2)
    return 0;

  len = len/2;
  while(i < len && i < bufferlen){
    int hi = c[0];
    int lo = c[1];
    hi = HEXTOINT(hi);
    lo = HEXTOINT(lo);

    out[0] = ((hi*16)+lo);

    out++;
    c+=2;
    i++;
  }
  return i;
}

LUX_API uint32 lxStrHash(const char *str)
{
  uint32 hash = 0;
  const unsigned char *p = (const unsigned char *)str;

#if 0
  for (; *p != '\0'; p++){
    hash = 65599 * hash + *p; //37
  }
#else
  unsigned char c;
  while (c = *p++){
    hash = c + (hash << 6) + (hash << 16) - hash;
  }
#endif
  return hash; 
}

//-----------------------------------------------------------------------------
// MurmurHash2, by Austin Appleby
// For business purposes, Murmurhash is under the MIT license. 

// Note - This code makes a few assumptions about how your machine behaves -

// 1. We can read a 4-byte value from any address without crashing
// 2. sizeof(int) == 4

// And it has a few limitations -

// 1. It will not work incrementally.
// 2. It will not produce the same results on little-endian and big-endian
//    machines.

LUX_API uint32 lxStrMurmurHash2 ( const void * key, size_t len, uint32 seed )
{
  // 'm' and 'r' are mixing constants generated offline.
  // They're not really 'magic', they just happen to work well.

  const unsigned int m = 0x5bd1e995;
  const int r = 24;

  // Initialize the hash to a 'random' value

  unsigned int h = seed ^ len;

  // Mix 4 bytes at a time into the hash

  const unsigned char * data = (const unsigned char *)key;

  while(len >= 4)
  {
    unsigned int k = *(unsigned int *)data;

    k *= m; 
    k ^= k >> r; 
    k *= m; 

    h *= m; 
    h ^= k;

    data += 4;
    len -= 4;
  }

  // Handle the last few bytes of the input array

  switch(len)
  {
  case 3: h ^= data[2] << 16;
  case 2: h ^= data[1] << 8;
  case 1: h ^= data[0];
    h *= m;
  };

  // Do a few final mixes of the hash to ensure the last few
  // bytes are well-incorporated.

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
} 

//-----------------------------------------------------------------------------
// MurmurHash2A, by Austin Appleby

// This is a variant of MurmurHash2 modified to use the Merkle-Damgard
// construction. Bulk speed should be identical to Murmur2, small-key speed
// will be 10%-20% slower due to the added overhead at the end of the hash.

// This variant fixes a minor issue where null keys were more likely to
// collide with each other than expected, and also makes the algorithm
// more amenable to incremental implementations. All other caveats from
// MurmurHash2 still apply.

#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

LUX_API uint32 lxStrMurmurHash2A ( const void * key, size_t len, uint32 seed )
{
  const unsigned int m = 0x5bd1e995;
  const int r = 24;
  unsigned int l = len;

  const unsigned char * data = (const unsigned char *)key;

  unsigned int h = seed;
  unsigned int t = 0;

  while(len >= 4)
  {
    unsigned int k = *(unsigned int*)data;

    mmix(h,k);

    data += 4;
    len -= 4;
  }

  

  switch(len)
  {
  case 3: t ^= data[2] << 16;
  case 2: t ^= data[1] << 8;
  case 1: t ^= data[0];
  };

  mmix(h,t);
  mmix(h,l);

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}



/**************************************************************************
 *
 * Copyright 2008 VMware, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL VMWARE AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/

/**
 * @file
 * Hash functions implementation.
 * 
 * @author Jose Fonseca
 */

static const uint32 
util_crc32_table[256] = {
   0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 
   0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 
   0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 
   0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 
   0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 
   0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 
   0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 
   0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 
   0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 
   0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 
   0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 
   0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 
   0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 
   0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f, 
   0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 
   0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 
   0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 
   0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433, 
   0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 
   0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01, 
   0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 
   0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 
   0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 
   0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 
   0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 
   0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 
   0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 
   0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 
   0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 
   0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f, 
   0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 
   0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad, 
   0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 
   0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 
   0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 
   0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1, 
   0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 
   0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 
   0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 
   0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 
   0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 
   0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b, 
   0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 
   0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 
   0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 
   0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 
   0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 
   0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 
   0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 
   0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713, 
   0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 
   0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 
   0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 
   0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777, 
   0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 
   0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45, 
   0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 
   0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 
   0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 
   0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9, 
   0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 
   0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 
   0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 
   0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};


/**
 * @sa http://www.w3.org/TR/PNG/#D-CRCAppendix
 */
LUX_API uint32 lxStrHashCrc32(const void *data, size_t size)
{
   const uint8 *p = (const uint8 *)data;
   uint32 crc = 0xffffffff;
 
   while (size--)
      crc = util_crc32_table[(crc ^ *p++) & 0xff] ^ (crc >> 8);
   
   return crc;
}

LUX_API uint32 lxStrHashSimple(const char *key)
{
  const char *str = key;
  uint32 hash = 0;
  while (*str != '\0') {
    uint32 g;

    hash = (hash << 4) + (uint32) (*str++);
    g = hash & 0xf0000000;
    if (g != 0)
      hash ^= g >> 24;
    hash &= ~g;
  }
  return hash;
}
