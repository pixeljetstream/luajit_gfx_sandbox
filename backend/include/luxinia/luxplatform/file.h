// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h



#ifndef __LUXCORE_FILE_H__
#define __LUXCORE_FILE_H__

#include <luxinia/luxplatform/luxplatform.h>

#ifdef __cplusplus
extern "C"{
#endif

  typedef struct lxFSFile_s
  {
    byte  *contents;
    size_t  index;
    size_t  size;

    booln freecontent;
  }lxFSFile_t;

  LUX_API booln   lxFS_setCurrent(lxFSFile_t *file, void *data);
  LUX_API void*   lxFS_getCurrent(lxFSFile_t *file);
  LUX_API void*   lxFS_getContent(lxFSFile_t *file);
  LUX_API size_t  lxFS_getSize(lxFSFile_t *file);


  LUX_API char*   lxFS_gets(char *buffer, size_t size, lxFSFile_t *file);
  LUX_API char    lxFS_eof(lxFSFile_t *file);
  LUX_API char    lxFS_getc(lxFSFile_t *file);
  LUX_API size_t  lxFS_read(void* target, size_t size, size_t count,lxFSFile_t *file);
  LUX_API size_t  lxFS_seek(lxFSFile_t* file, size_t offset, size_t origin);

#ifdef __cplusplus
}
#endif

#endif
