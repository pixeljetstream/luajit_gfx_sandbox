/*
  Copyright (C) 2008 Martin Storsjö

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/*
  Christoph Kubisch

  Changes to original implementation: 
   * VertexCacheSize now maps to a variable, which resulted into a few runtime
    changes.
   * Templated for different VertexIndexTypes
   * inplace operations

  Original Algorithm:
  http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html
*/

#include <luxinia/luxscene/meshvcacheopt.h>

#include <math.h>
#include <string.h>
#include <assert.h>

// Set these to adjust the performance and result quality
#define VERTEX_CACHE_SIZE   l_vcachesize
#define VERTEX_CACHE_SIZE_MAX 32
#define CACHE_FUNCTION_LENGTH 32

// The size of these data types affect the memory usage
typedef uint16 ScoreType;
#define SCORE_SCALING 7281

typedef uint8 AdjacencyType;
#define MAX_ADJACENCY 255


typedef int8  CachePosType;
typedef int32 TriangleIndexType;
typedef int32 ArrayIndexType;

// The size of the precalculated tables
#define CACHE_SCORE_TABLE_SIZE    32
#define VALENCE_SCORE_TABLE_SIZE  32

// Precalculated tables
static ScoreType l_cachePositionScore[CACHE_SCORE_TABLE_SIZE];
static ScoreType l_valenceScore[VALENCE_SCORE_TABLE_SIZE];
static int    l_inited = 0;
static int    l_vcachesize = 8;

#define ISADDED(x)  (triangleAdded[(x) >> 3] &  (1 << (x & 7)))
#define SETADDED(x) (triangleAdded[(x) >> 3] |= (1 << (x & 7)))

// Score function constants
#define CACHE_DECAY_POWER 1.5f
#define LAST_TRI_SCORE    0.75f
#define VALENCE_BOOST_SCALE 2.0f
#define VALENCE_BOOST_POWER 0.5f

// Precalculate the tables
static int initForsyth() {
  if (VERTEX_CACHE_SIZE > CACHE_SCORE_TABLE_SIZE   ||
    VERTEX_CACHE_SIZE > VERTEX_CACHE_SIZE_MAX)
    return 0;

  for (int i = 0; i < CACHE_SCORE_TABLE_SIZE; i++) {
    float score = 0;
    if (i < 3) {
      // This vertex was used in the last triangle,
      // so it has a fixed score, which ever of the three
      // it's in. Otherwise, you can get very different
      // answers depending on whether you add
      // the triangle 1,2,3 or 3,1,2 - which is silly
      score = LAST_TRI_SCORE;
    } else {
      // Points for being high in the cache.
      const float scaler = 1.0f / (CACHE_FUNCTION_LENGTH - 3);
      score = 1.0f - (i - 3) * scaler;
      score = powf(score, CACHE_DECAY_POWER);
    }
    l_cachePositionScore[i] = (ScoreType) (SCORE_SCALING * score);
  }

  for (int i = 1; i < VALENCE_SCORE_TABLE_SIZE; i++) {
    // Bonus points for having a low number of tris still to
    // use the vert, so we get rid of lone verts quickly
    float valenceBoost = powf((float)i, -VALENCE_BOOST_POWER);
    float score = VALENCE_BOOST_SCALE * valenceBoost;
    l_valenceScore[i] = (ScoreType) (SCORE_SCALING * score);
  }

  l_inited = l_vcachesize;
  return 1;
}

// Calculate the score for a vertex
static ScoreType findVertexScore(int numActiveTris,
                          int cachePosition) {
  if (numActiveTris == 0) {
    // No triangles need this vertex!
    return 0;
  }

  ScoreType score = 0;
  if (cachePosition < 0) {
    // Vertex is not in LRU cache - no score
  } else {
    score = l_cachePositionScore[cachePosition];
  }

  if (numActiveTris < VALENCE_SCORE_TABLE_SIZE)
    score += l_valenceScore[numActiveTris];
  return score;
}

// The main reordering function
template<class VertexIndexType>
VertexIndexType* TreorderForsyth(VertexIndexType* indices,
                                int nTriangles,
                                int nVertices,
                int vcache) {

  // The tables need not be inited every time this function
  // is used. Either call initForsyth from the calling process,
  // or just replace the score tables with precalculated values.

  l_vcachesize = vcache;
  if (l_inited != l_vcachesize && !initForsyth())
    return NULL;

  AdjacencyType* numActiveTris = new AdjacencyType[nVertices];
  memset(numActiveTris, 0, sizeof(AdjacencyType)*nVertices);

  // First scan over the vertex data, count the total number of
  // occurrances of each vertex
  for (int i = 0; i < 3*nTriangles; i++) {
    if (numActiveTris[indices[i]] == MAX_ADJACENCY) {
      // Unsupported mesh,
      // vertex shared by too many triangles
      delete [] numActiveTris;
      return NULL;
    }
    numActiveTris[indices[i]]++;
  }

  // Allocate the rest of the arrays
  ArrayIndexType* offsets = new ArrayIndexType[nVertices];
  ScoreType* lastScore = new ScoreType[nVertices];
  CachePosType* cacheTag = new CachePosType[nVertices];

  uint8* triangleAdded = new uint8[(nTriangles + 7)/8];
  ScoreType* triangleScore = new ScoreType[nTriangles];
  TriangleIndexType* triangleIndices = new TriangleIndexType[3*nTriangles];
  memset(triangleAdded, 0, sizeof(uint8)*((nTriangles + 7)/8));
  memset(triangleScore, 0, sizeof(ScoreType)*nTriangles);
  memset(triangleIndices, 0, sizeof(TriangleIndexType)*3*nTriangles);

  // Count the triangle array offset for each vertex,
  // initialize the rest of the data.
  int sum = 0;
  for (int i = 0; i < nVertices; i++) {
    offsets[i] = sum;
    sum += numActiveTris[i];
    numActiveTris[i] = 0;
    cacheTag[i] = -1;
  }

  // Fill the vertex data structures with indices to the triangles
  // using each vertex
  for (int i = 0; i < nTriangles; i++) {
    for (int j = 0; j < 3; j++) {
      int v = indices[3*i + j];
      triangleIndices[offsets[v] + numActiveTris[v]] = i;
      numActiveTris[v]++;
    }
  }

  // Initialize the score for all vertices
  for (int i = 0; i < nVertices; i++) {
    lastScore[i] = findVertexScore(numActiveTris[i], cacheTag[i]);
    for (int j = 0; j < numActiveTris[i]; j++)
      triangleScore[triangleIndices[offsets[i] + j]] += lastScore[i];
  }

  // Find the best triangle
  int bestTriangle = -1;
  int bestScore = -1;

  for (int i = 0; i < nTriangles; i++) {
    if (triangleScore[i] > bestScore) {
      bestScore = triangleScore[i];
      bestTriangle = i;
    }
  }

  // Allocate the output array
  TriangleIndexType* outTriangles = new TriangleIndexType[nTriangles];
  int outPos = 0;

  // Initialize the cache
  int cache[VERTEX_CACHE_SIZE_MAX + 3];
  for (int i = 0; i < VERTEX_CACHE_SIZE + 3; i++)
    cache[i] = -1;

  int scanPos = 0;

  // Output the currently best triangle, as long as there
  // are triangles left to output
  while (bestTriangle >= 0) {
    // Mark the triangle as added
    SETADDED(bestTriangle);
    // Output this triangle
    outTriangles[outPos++] = bestTriangle;
    for (int i = 0; i < 3; i++) {
      // Update this vertex
      int v = indices[3*bestTriangle + i];

      // Check the current cache position, if it
      // is in the cache
      int endpos = cacheTag[v];
      if (endpos < 0)
        endpos = VERTEX_CACHE_SIZE + i;
      // Move all cache entries from the previous position
      // in the cache to the new target position (i) one
      // step backwards
      for (int j = endpos; j > i; j--) {
        cache[j] = cache[j-1];
        // If this cache slot contains a real
        // vertex, update its cache tag
        if (cache[j] >= 0)
          cacheTag[cache[j]]++;
      }
      // Insert the current vertex into its new target
      // slot
      cache[i] = v;
      cacheTag[v] = i;

      // Find the current triangle in the list of active
      // triangles and remove it (moving the last
      // triangle in the list to the slot of this triangle).
      for (int j = 0; j < numActiveTris[v]; j++) {
        if (triangleIndices[offsets[v] + j] == bestTriangle) {
          triangleIndices[offsets[v] + j] =
              triangleIndices[
                  offsets[v] + numActiveTris[v] - 1];
          break;
        }
      }
      // Shorten the list
      numActiveTris[v]--;
    }
    // Update the scores of all triangles in the cache
    for (int i = 0; i < VERTEX_CACHE_SIZE + 3; i++) {
      int v = cache[i];
      if (v < 0)
        break;
      // This vertex has been pushed outside of the
      // actual cache
      if (i >= VERTEX_CACHE_SIZE) {
        cacheTag[v] = -1;
        cache[i] = -1;
      }
      ScoreType newScore = findVertexScore(numActiveTris[v],
                                           cacheTag[v]);
      ScoreType diff = newScore - lastScore[v];
      for (int j = 0; j < numActiveTris[v]; j++)
        triangleScore[triangleIndices[offsets[v] + j]] += diff;
      lastScore[v] = newScore;
    }
    // Find the best triangle referenced by vertices in the cache
    bestTriangle = -1;
    bestScore = -1;
    for (int i = 0; i < VERTEX_CACHE_SIZE; i++) {
      if (cache[i] < 0)
        break;
      int v = cache[i];
      for (int j = 0; j < numActiveTris[v]; j++) {
        int t = triangleIndices[offsets[v] + j];
        if (triangleScore[t] > bestScore) {
          bestTriangle = t;
          bestScore = triangleScore[t];
        }
      }
    }
    // If no active triangle was found at all, continue
    // scanning the whole list of triangles
    if (bestTriangle < 0) {
      for (; scanPos < nTriangles; scanPos++) {
        if (!ISADDED(scanPos)) {
          bestTriangle = scanPos;
          break;
        }
      }
    }
  }

  // Clean up
  delete [] triangleIndices;
  delete [] offsets;
  delete [] lastScore;
  delete [] numActiveTris;
  delete [] cacheTag;
  delete [] triangleAdded;
  delete [] triangleScore;

  // Convert the triangle index array into a full triangle list
  VertexIndexType* outIndices = new VertexIndexType[3*nTriangles];
  outPos = 0;
  for (int i = 0; i < nTriangles; i++) {
    int t = outTriangles[i];
    for (int j = 0; j < 3; j++) {
      int v = indices[3*t + j];
      outIndices[outPos++] = v;
    }
  }
  outPos = 0;
  for (int i = 0; i < nTriangles; i++) {
    indices[outPos+0] = outIndices[outPos+0];
    indices[outPos+1] = outIndices[outPos+1];
    indices[outPos+2] = outIndices[outPos+2];
    outPos += 3;
  }

  delete [] outIndices;
  delete [] outTriangles;

  return indices;
}

LUX_API void* lxVertexCacheOptimize_forsyth(void* indices,
        int nTriangles,
        int nVertices,
        int vcache,
        lxMeshIndexType_t type )
{
  switch(type){
  case LUX_MESH_INDEX_UINT16:
    return TreorderForsyth<uint16>((uint16*)indices,nTriangles,nVertices,vcache);
    break;
  case LUX_MESH_INDEX_UINT32:
    return TreorderForsyth<uint32>((uint32*)indices,nTriangles,nVertices,vcache);
  default:
    return NULL;
  }
}
