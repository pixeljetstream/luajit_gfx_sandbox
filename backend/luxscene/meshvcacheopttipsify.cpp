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
  http://www.cs.princeton.edu/gfx/pubs/Sander_2007_%3ETR/tipsy.pdf
*/

#include <luxinia/luxscene/meshvcacheopt.h>

#include <stdlib.h>
#include <string.h>

#define DEAD_END_STACK_SIZE 128
#define DEAD_END_STACK_MASK (DEAD_END_STACK_SIZE - 1)


// The size of these data types control the memory usage
typedef uint8 AdjacencyType;
#define MAX_ADJACENCY 255

typedef int32 TriangleIndexType;
typedef int32 ArrayIndexType;

#define ISEMITTED(x)  (emitted[(x) >> 3] &  (1 << (x & 7)))
#define SETEMITTED(x) (emitted[(x) >> 3] |= (1 << (x & 7)))

// Find the next non-local vertex to continue from
template<class VertexIndexType>
static int TskipDeadEnd(const AdjacencyType* liveTriangles,
                const VertexIndexType* deadEndStack,
                int& deadEndStackPos,
                int& deadEndStackStart,
                int nVertices,
                int& i) {

  // Next in dead-end stack
  while ((deadEndStackPos & DEAD_END_STACK_MASK) != deadEndStackStart) {
    int d = deadEndStack[(--deadEndStackPos) & DEAD_END_STACK_MASK];
    // Check for live triangles
    if (liveTriangles[d] > 0)
      return d;
  }
  // Next in input order
  while (i + 1 < nVertices) {
    // Cursor sweeps list only once
    i++;
    // Check for live triangles
    if (liveTriangles[i] > 0)
      return i;
  }
  // We are done!
  return -1;
}

// Find the next vertex to continue from
template<class VertexIndexType>
static int TgetNextVertex(int nVertices,
                  int& i,
                  int k,
                  const VertexIndexType* nextCandidates,
                  int numNextCandidates,
                  const ArrayIndexType* cacheTime,
                  int s,
                  const AdjacencyType* liveTriangles,
                  const VertexIndexType* deadEndStack,
                  int& deadEndStackPos,
                  int& deadEndStackStart) {

  // Best candidate
  int n = -1;
  // and priority
  int m = -1;
  for (int j = 0; j < numNextCandidates; j++) {
    int v = nextCandidates[j];
    // Must have live triangles
    if (liveTriangles[v] > 0) {
      // Initial priority
      int p = 0;
      // In cache even after fanning?
      if (s - cacheTime[v] + 2*liveTriangles[v] <= k)
        // Priority is position in cache
        p = s - cacheTime[v];
      // Keep best candidate
      if (p > m) {
        m = p;
        n = v;
      }
    }
  }
  // Reached a dead-end?
  if (n == -1) {
    // Get non-local vertex
    n = TskipDeadEnd<VertexIndexType>(liveTriangles, deadEndStack,
                    deadEndStackPos, deadEndStackStart,
                    nVertices, i);
  }
  return n;
}

// The main reordering function
template<class VertexIndexType>
VertexIndexType* Ttipsify(VertexIndexType* indices,
                         int nTriangles,
                         int nVertices,
                         int k) 
{
  // Vertex-triangle adjacency

  // Count the occurrances of each vertex
  AdjacencyType* numOccurrances = new AdjacencyType[nVertices];
  memset(numOccurrances, 0, sizeof(AdjacencyType)*nVertices);
  for (int i = 0; i < 3*nTriangles; i++) {
    int v = indices[i];
    if (numOccurrances[v] == MAX_ADJACENCY) {
      // Unsupported mesh,
      // vertex shared by too many triangles
      delete [] numOccurrances;
      return NULL;
    }
    numOccurrances[v]++;
  }

  // Find the offsets into the adjacency array for each vertex
  int sum = 0;
  ArrayIndexType* offsets = new ArrayIndexType[nVertices+1];
  int maxAdjacency = 0;
  for (int i = 0; i < nVertices; i++) {
    offsets[i] = sum;
    sum += numOccurrances[i];
    if (numOccurrances[i] > maxAdjacency)
      maxAdjacency = numOccurrances[i];
    numOccurrances[i] = 0;
  }
  offsets[nVertices] = sum;

  // Add the triangle indices to the vertices it refers to
  TriangleIndexType* adjacency = new TriangleIndexType[3*nTriangles];
  for (int i = 0; i < nTriangles; i++) {
    const VertexIndexType* vptr = &indices[3*i];
    adjacency[offsets[vptr[0]] + numOccurrances[vptr[0]]] = i;
    numOccurrances[vptr[0]]++;
    adjacency[offsets[vptr[1]] + numOccurrances[vptr[1]]] = i;
    numOccurrances[vptr[1]]++;
    adjacency[offsets[vptr[2]] + numOccurrances[vptr[2]]] = i;
    numOccurrances[vptr[2]]++;
  }

  // Per-vertex live triangle counts
  AdjacencyType* liveTriangles = numOccurrances;

  // Per-vertex caching time stamps
  ArrayIndexType* cacheTime = new ArrayIndexType[nVertices];
  memset(cacheTime, 0, sizeof(ArrayIndexType)*nVertices);

  // Dead-end vertex stack
  VertexIndexType* deadEndStack = new VertexIndexType[DEAD_END_STACK_SIZE];
  memset(deadEndStack, 0, sizeof(VertexIndexType)*DEAD_END_STACK_SIZE);
  int deadEndStackPos = 0;
  int deadEndStackStart = 0;

  // Per triangle emitted flag
  uint8* emitted = new uint8[(nTriangles + 7)/8];
  memset(emitted, 0, sizeof(uint8)*((nTriangles + 7)/8));

  // Empty output buffer
  TriangleIndexType* outputTriangles = new TriangleIndexType[nTriangles];
  int outputPos = 0;

  // Arbitrary starting vertex
  int f = 0;
  // Time stamp and cursor
  int s = k + 1;
  int id = 0;

  VertexIndexType* nextCandidates = new VertexIndexType[3*maxAdjacency];

  // For all valid fanning vertices
  while (f >= 0) {
    // 1-ring of next candidates
    int numNextCandidates = 0;
    int startOffset = offsets[f];
    int endOffset = offsets[f+1];
    for (int offset = startOffset; offset < endOffset; offset++) {
      int t = adjacency[offset];
      if (!ISEMITTED(t)) {
        const VertexIndexType* vptr = &indices[3*t];
        // Output triangle
        outputTriangles[outputPos++] = t;
        for (int j = 0; j < 3; j++) {
          int v = vptr[j];
          // Add to dead-end stack
          deadEndStack[(deadEndStackPos++) & DEAD_END_STACK_MASK] = v;
          if ((deadEndStackPos & DEAD_END_STACK_MASK) ==
              (deadEndStackStart & DEAD_END_STACK_MASK))
            deadEndStackStart = (deadEndStackStart + 1) & DEAD_END_STACK_MASK;
          // Register as candidate
          nextCandidates[numNextCandidates++] = v;
          // Decrease live triangle count
          liveTriangles[v]--;
          // If not in cache
          if (s - cacheTime[v] > k) {
            // Set time stamp
            cacheTime[v] = s;
            // Increment time stamp
            s++;
          }
        }
        // Flag triangle as emitted
        SETEMITTED(t);
      }
    }
    // Select next fanning vertex
    f = TgetNextVertex<VertexIndexType>(nVertices, id, k, nextCandidates,
                      numNextCandidates, cacheTime, s,
                      liveTriangles, deadEndStack,
                      deadEndStackPos, deadEndStackStart);
  }

  // Clean up
  delete [] nextCandidates;
  delete [] emitted;
  delete [] deadEndStack;
  delete [] cacheTime;
  delete [] adjacency;
  delete [] offsets;
  delete [] numOccurrances;

  // Convert the triangle index array into a full triangle list
  VertexIndexType* outputIndices = new VertexIndexType[3*nTriangles];
  outputPos = 0;
  for (int i = 0; i < nTriangles; i++) {
    int t = outputTriangles[i];
    for (int j = 0; j < 3; j++) {
      int v = indices[3*t + j];
      outputIndices[outputPos++] = v;
    }
  }
  outputPos = 0;
  for (int i = 0; i < nTriangles; i++) {
    indices[outputPos+0] = outputIndices[outputPos+0];
    indices[outputPos+1] = outputIndices[outputPos+1];
    indices[outputPos+2] = outputIndices[outputPos+2];
    outputPos += 3;
  }

  delete [] outputTriangles;
  delete [] outputIndices;

  return indices;
}


LUX_API void* lxVertexCacheOptimize_tipsify(void* indices,
        int nTriangles,
        int nVertices,
        int k,
        lxMeshIndexType_t type )
{
  switch(type){
  case LUX_MESH_INDEX_UINT16:
    return Ttipsify<uint16>((uint16*)indices,nTriangles,nVertices,k);
    break;
  case LUX_MESH_INDEX_UINT32:
    return Ttipsify<uint32>((uint32*)indices,nTriangles,nVertices,k);
  default:
    return NULL;
  }
}