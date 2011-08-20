/*
  Optimal Grid VertexCache, by Igancio Castano
  http://castano.ludicon.com/blog/2009/02/02/optimal-grid-rendering
*/

#include <luxinia/luxscene/meshvcacheopt.h>

template<class VertexIndexType>
VertexIndexType* TgridGen(VertexIndexType* pcurrent, VertexIndexType* pend, int x0, int x1, int y0, int y1, int width, int cacheSize)
{
  if (x1 - x0 + 1 < cacheSize)
  {
    if (2 * (x1 - x0) + 1 > cacheSize)
    {
      if (pcurrent+((x1-x0)*3) > pend)
        return NULL;

      for (int x = x0; x < x1; x++, pcurrent+=3)
      {
        pcurrent[0] = x + 0;
        pcurrent[1] = x + 0;
        pcurrent[2] = x + 1;
      }
    }

    if (pcurrent+( (y1-y0)*(x1-x0)*6) > pend)
      return NULL;

    for (int y = y0; y < y1; y++)
    {
      for (int x = x0; x < x1; x++, pcurrent+=6)
      {
        pcurrent[0] = ((width + 1) * (y + 0) + (x + 0));
        pcurrent[1] = ((width + 1) * (y + 1) + (x + 0));
        pcurrent[2] = ((width + 1) * (y + 0) + (x + 1));

        pcurrent[3] = ((width + 1) * (y + 0) + (x + 1));
        pcurrent[4] = ((width + 1) * (y + 1) + (x + 0));
        pcurrent[5] = ((width + 1) * (y + 1) + (x + 1));
      }
    }
  }
  else
  {
    int xm = x0 + cacheSize - 2;
    pcurrent = TgridGen<VertexIndexType>(pcurrent,pend,x0, xm, y0, y1, width, cacheSize);
    if (!pcurrent)
      return NULL;
    pcurrent = TgridGen<VertexIndexType>(pcurrent,pend,xm, x1, y0, y1, width, cacheSize);
  }

  return pcurrent;
}


LUX_API void* lxVertexCacheOptimize_grid_castano(
  void* indices,  int maxTriangles,
  int width,  int height,
  int vcache, lxMeshIndexType_t type,
  int *writtenTriangles)
{
  int x0 = 0;
  int x1 = width;
  int y0 = 0;
  int y1 = height;

  switch(type){
  case LUX_MESH_INDEX_UINT16:
    {
      uint16* pstart = (uint16*)indices;
      uint16* pend = pstart + (maxTriangles*3);
      uint16* pwritten = TgridGen<uint16>(pstart,pend,x0, x1, y0, y1, width, vcache);

      *writtenTriangles = pwritten ? (pwritten - pstart)/3 : 0;

      return pwritten ? pstart : NULL;
    }
    
  case LUX_MESH_INDEX_UINT32:
    {
      uint32* pstart = (uint32*)indices;
      uint32* pend = pstart + (maxTriangles*3);
      uint32* pwritten = TgridGen<uint32>(pstart,pend,x0, x1, y0, y1, width, vcache);

      *writtenTriangles = pwritten ? (pwritten - pstart)/3 : 0;

      return pwritten ? pstart : NULL;
    }
  default:
    *writtenTriangles = 0;
    return NULL;
  }
}

