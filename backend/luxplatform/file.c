// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxplatform/file.h>
#include <stdio.h>
#include <string.h>

LUX_API char lxFS_getc(lxFSFile_t *file){
  char c = file->contents[file->index];
  if (file->index >= file->size)
    return EOF;
  file->index++;
  return c;
}

LUX_API size_t lxFS_read(void* target, size_t size, size_t count,lxFSFile_t *file){
  size_t read;
  // find out how many we can copy
  if (file->size <= file->index)
    return 0;
  else if ((unsigned long)(file->size - file->index) >= size*count)
    read = (size*count);
  else
    read = (size*count)%((file->size)-(file->index));
  read /= size;

  memcpy(target,&file->contents[file->index],read*size);
  file->index+=read*size;
  return read;
}

LUX_API size_t lxFS_seek(lxFSFile_t* file, size_t offset, size_t origin)
{
  long oldoffset = file->index;

  switch(origin)
  {
  case SEEK_SET:
    file->index = offset;
    break;

  case SEEK_CUR:
    file->index += offset;
    break;

  case SEEK_END:
    file->index = file->size - offset;
    break;

  default:
    return LUX_FALSE;
  }

  if(offset < 0 || offset > file->size)
  {
    file->index = oldoffset;
    return LUX_FALSE;
  }

  return LUX_TRUE;
}

LUX_API char * lxFS_gets(char *buffer, size_t size, lxFSFile_t *file)
{
  size_t i;
  char *string = (char*)&file->contents[file->index];

  for(i=0; i < size && i+file->index < file->size; i++)
  {
    buffer[i] = string[i];
    if(string[i] == '\0' || string[i] == '\n')
    {
      i++;
      break;
    }
  }

  buffer[i] = '\0';

  file->index += i;

  if(i == 0)
    return NULL;
  else
    return buffer;
}

LUX_API char lxFS_eof(lxFSFile_t *fsfile){
  if (!fsfile)
    return LUX_TRUE;
  return fsfile->index >= fsfile->size;
}

LUX_API void* lxFS_getCurrent(lxFSFile_t *file)
{
  if(!file || file->index >= file->size) return NULL;

  return &file->contents[file->index];
}

LUX_API booln lxFS_setCurrent(lxFSFile_t *file, void* cur)
{
  byte *data = (byte*)cur;
  if (data < file->contents || data > file->contents+file->size)
    return LUX_FALSE;

  file->index = data-file->contents;

  return LUX_TRUE;
}

LUX_API void *lxFS_getContent(lxFSFile_t *file)
{
  if(!file) return NULL;

  return file->contents;
}


LUX_API size_t lxFS_getSize(lxFSFile_t *file)
{
  if(!file) return 0;

  return file->size;
}
