// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// For conditions of distribution and use, see copyright notice in LICENSE.txt
//
// Forwards to Code by Martin Storsjö, Ignacio Castanio, Tom Forsyth

#ifndef __LUXCORE_MESHVCACHEOPT_H__
#define __LUXCORE_MESHVCACHEOPT_H__

#include <luxplatform/luxplatform.h>

#if defined(__cplusplus)
extern "C"
{
#endif

//////////////////////////////////////////////////////////////////////////
// Vertex Cache Optimizations

typedef enum lxVertexIndexType_e{
  VERTEX_INDEX_TYPE_UINT16,
  VERTEX_INDEX_TYPE_UINT32, // only within signed range
}lxVertexIndexType_t;

// Original Algorithm:
// http://www.cs.princeton.edu/gfx/pubs/Sander_2007_%3ETR/tipsy.pdf
LUX_API void* lxVertexCacheOptimize_tipsify(
  void* indices,
  int nTriangles,
  int nVertices,
  int k,
  lxVertexIndexType_t type );


// Original Algorithm:
// http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html
//
// NOT THREADSAFE!!
// vcachesize <= 32
LUX_API void* lxVertexCacheOptimize_forsyth(
  void* indices,
  int nTriangles,
  int nVertices,
  int vcache,
  lxVertexIndexType_t type );


// Optimal Grid VertexCache, by Igancio Castano
// http://castano.ludicon.com/blog/2009/02/02/optimal-grid-rendering
//
// if buffer was too small, NULL
// is returned
LUX_API void* lxVertexCacheOptimize_grid_castano(
  void* indices,
  int maxTriangles,
  int width,
  int height,
  int vcache,
  lxVertexIndexType_t type,
  int *writtenTriangles);

#if defined(__cplusplus)
}
#endif

#endif
