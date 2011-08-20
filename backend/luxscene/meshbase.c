// Copyright (C) 2004-2011 Christoph Kubisch
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxscene/meshbase.h>
#include <luxinia/luxmath/matrix44.h>



// segs = x,y
LUX_API void lxMeshPlane_getCounts(int segs[2], int* numVertices, int* numTriangleIndices, int* numOutlineIndices)
{
  *numVertices = (segs[0] + 1) * (segs[1] + 1);
  *numTriangleIndices = (segs[0] * segs[1] * 2) * 3;
  *numOutlineIndices = ((segs[0] * 2) + (segs[1] * 2)) * 2;
}

LUX_API void lxMeshPlane_initTriangles(int segs[2], lxVector3* pos, lxVector3* normal, lxVector2* uv, uint32* indices)
{
  int xdim = segs[0];
  int ydim = segs[1];
  
  float xmove = 1.0f/(float)segs[0];
  float ymove = 1.0f/(float)segs[1];

  int vert = 0;
  int idx = 0;
  int width = (xdim + 1);

  int x,y;
  for (y = 0; y < ydim + 1; y++){
    for (x = 0; x < xdim + 1; x++, vert++){
      float xpos = ((float)x * xmove);
      float ypos = ((float)y * ymove);
      pos[vert][0] = (xpos - 0.5f) * 2.0f;
      pos[vert][1] = (ypos - 0.5f) * 2.0f;
      pos[vert][2] = 0;

      uv[vert][0] = xpos;
      uv[vert][1] = ypos;

      normal[vert][0] = 0.0f;
      normal[vert][1] = 0.0f;
      normal[vert][2] = -1.0f;
    }
  }

  for (y = 0; y < ydim; y++){
    for (x = 0; x < xdim; x++){
      // lower tris
      indices[idx++] = (x)      + (y)     * width;
      indices[idx++] = (x + 1)  + (y)     * width;
      indices[idx++] = (x + 1)  + (y + 1) * width;

      // upper tris
      indices[idx++] = (x + 1)  + (y + 1) * width;
      indices[idx++] = (x)      + (y + 1) * width;
      indices[idx++] = (x)      + (y)     * width;
    }
  }
}

LUX_API void lxMeshPlane_initOutline(int segs[2], uint32* indices)
{
  int xdim = segs[0];
  int ydim = segs[1];

  int idx = 0;
  int width = (xdim + 1);
  int x,y;
  for (y = 0; y < ydim; y++){
    indices[idx++] = (y)     * width;
    indices[idx++] = (y + 1) * width;
  }
  for (y = 0; y < ydim; y++){
    indices[idx++] = (y)     * width + xdim;
    indices[idx++] = (y + 1) * width + xdim;
  }
  for (x = 0; x < xdim; x++){
    indices[idx++] = (x)    ;
    indices[idx++] = (x + 1);
  }
  for (x = 0; x < xdim; x++){
    indices[idx++] = (x)     + ydim * width;
    indices[idx++] = (x + 1) + ydim * width;
  }
}

// segs = outer (x,y),cap
LUX_API void lxMeshDisc_getCounts(int segs[2], int* numVertices, int* numTriangleIndices, int* numOutlineIndices)
{
  *numVertices = (segs[0]) + ((segs[1] - 1) * segs[0]) + 1;
  *numTriangleIndices = ((segs[0] * (segs[1]-1) * 2) + segs[0]) * 3;
  *numOutlineIndices = segs[0] * 2;
}

LUX_API void lxMeshDisc_initTriangles(int segs[2], lxVector3* pos, lxVector3* normal, lxVector2* uv, uint32* indices)
{
  int odim = segs[0];
  int idim = segs[1];
  float oshift = LUX_MUL_TWOPI / (float)odim;
  float ishift = 1.0f / (float)idim;
  int i,o;

  int vertex = 0;
  int index = 0;
  // center
  pos[vertex][0] = 0.0f;
  pos[vertex][1] = 0.0f;
  pos[vertex][2] = 0.0f;

  normal[vertex][0] = 0.0f;
  normal[vertex][1] = 0.0f;
  normal[vertex][2] = -1.0f;

  uv[vertex][0] = 0.5f;
  uv[vertex][1] = 0.5f;
  vertex++;

  // rings
  for (i = 1; i <= idim; i++){
    for (o = 0; o < odim; o++, vertex++){
      float angle = oshift * (float)o;
      float xpos = cosf(angle) * ishift * (float)i;
      float ypos = sinf(angle) * ishift * (float)i;
      pos[vertex][0] = xpos;
      pos[vertex][1] = ypos;
      pos[vertex][2] = 0.0f;

      normal[vertex][0] = 0.0f;
      normal[vertex][1] = 0.0f;
      normal[vertex][2] = -1.0f;

      uv[vertex][0] = xpos * 0.5f + 0.5f;
      uv[vertex][1] = ypos * 0.5f + 0.5f;
    }
  }

  vertex = 1;
  for (i = 0; i < idim; i++){
    int vertexstart = vertex;
    for (o = 0; o < odim; o++, vertex++){
      int vertexnext = (o != odim - 1) ? vertex + 1 : vertexstart;
      if (i == 0){
        indices[index++] = vertex;
        indices[index++] = vertexnext;
        indices[index++] = 0;
      }
      else{
        indices[index++] = vertex;
        indices[index++] = vertexnext;
        indices[index++] = vertex - odim;

        indices[index++] = vertex - odim;
        indices[index++] = vertexnext;
        indices[index++] = vertexnext - odim;
      }
    }
  }
}

LUX_API void lxMeshDisc_initOutline(int segs[2], uint32* indices)
{
  int odim = segs[0];
  int idim = segs[1];

  int index = 0;
  int vertex = 1;
  int i,o;
  for (i = 0; i < idim; i++){
    int vertexstart = vertex;
    for (o = 0; o < odim; o++, vertex++){
      int vertexnext = (o != odim - 1) ? vertex + 1 : vertexstart;
      if (i == idim-1){
        indices[index++] = vertex;
        indices[index++] = vertexnext;
      }
    }
  }
}

LUX_API void lxMeshBox_getCounts(int segs[3], int* numVertices, int* numTriangleIndices, int* numOutlineIndices)
{
  int used[2];
  int numv;
  int numtris;
  int numline;

  used[0] = segs[0];
  used[1] = segs[1];
  lxMeshPlane_getCounts(used,&numv,&numtris,&numline);
  *numVertices = numv * 2;
  *numTriangleIndices = numtris * 2;
  *numOutlineIndices  = numline * 2;

  used[0] = segs[0];
  used[1] = segs[2];
  lxMeshPlane_getCounts(used,&numv,&numtris,&numline);
  *numVertices += numv * 2;
  *numTriangleIndices += numtris * 2;
  *numOutlineIndices  += numline * 2;

  used[0] = segs[1];
  used[1] = segs[2];
  lxMeshPlane_getCounts(used,&numv,&numtris,&numline);
  *numVertices += numv * 2;
  *numTriangleIndices += numtris * 2;
  *numOutlineIndices  += numline * 2;
}

static void lxMeshHelper_applyTransform(int num, lxVector3* pos, lxVector3* normal, lxMatrix44 matrix)
{
  lxVector3 nrm = {0.0f, 0.0f, -1.0f};
  int i;
  for (i = 0; i < num; i++){
    lxVector3 vec = {pos[i][0], pos[i][1], -1.0f};
    lxVector3Transform(pos[i],    vec, matrix);
    lxVector3Transform(normal[i], nrm, matrix);
  }
}

static void lxMeshHelper_applyOffset(int num, uint32* indices, int offset)
{
  int i;
  for (i = 0; i < num; i++){
    indices[i] += offset;
  }
}

LUX_API void lxMeshBox_initTriangles(int segs[3], lxVector3* pos, lxVector3* normal, lxVector2* uv, uint32* indices)
{
  lxMatrix44 matrix;
  int numv;
  int numtris;
  int numline;

  int offsetv = 0;
  int offsettris = 0;
  int configs[6][2] = {
    {segs[0],segs[1]},
    {segs[0],segs[1]},

    {segs[2],segs[1]},
    {segs[2],segs[1]},

    {segs[0],segs[2]},
    {segs[0],segs[2]},
  };

  int side;
  for (side = 0; side < 6; side++){
    lxVector3 angles;
    switch (side)
    {
    case 0:
      lxMatrix44Identity(matrix);
      break;
    case 1:
      lxVector3Set(angles,0,LUX_MUL_PI,0);
      lxMatrix44FromEulerXYZ(matrix,angles);
      break;
    case 2:
      lxVector3Set(angles,0,LUX_MUL_HALF_PI,0);
      lxMatrix44FromEulerXYZ(matrix,angles);
      break;
    case 3:
      lxVector3Set(angles,0,LUX_MUL_PI * 1.5f,0);
      lxMatrix44FromEulerXYZ(matrix,angles);
      break;
    case 4:
      lxVector3Set(angles,LUX_MUL_HALF_PI,0,0);
      lxMatrix44FromEulerXYZ(matrix,angles);
      break;
    case 5:
      lxVector3Set(angles,LUX_MUL_PI * 1.5f,0,0);
      lxMatrix44FromEulerXYZ(matrix,angles);
      break;
    }

    lxMeshPlane_getCounts(configs[side],&numv,&numtris,&numline);
    lxMeshPlane_initTriangles(configs[side],pos + offsetv,normal + offsetv,uv + offsetv,indices + offsettris);
    lxMeshHelper_applyOffset(numtris,indices + offsettris,offsetv);
    lxMeshHelper_applyTransform(numv,pos + offsetv,normal + offsetv,matrix);

    offsetv += numv;
    offsettris += numtris;
  }
}

LUX_API void lxMeshBox_initOutline(int segs[3], uint32* indices)
{
  int numv;
  int numtris;
  int numline;

  int offsetv = 0;
  int offsetline = 0;
  int configs[6][2] = {
    {segs[0],segs[1]},
    {segs[0],segs[1]},

    {segs[2],segs[1]},
    {segs[2],segs[1]},

    {segs[0],segs[2]},
    {segs[0],segs[2]},
  };

  int side;
  for (side = 0; side < 6; side++){
    lxMeshPlane_getCounts(configs[side],&numv,&numtris,&numline);
    lxMeshPlane_initOutline(configs[side],indices + offsetline);
    lxMeshHelper_applyOffset(numline,indices + offsetline,offsetv);
    offsetline += numline;
    offsetv += numv;
  }
}

// segs = x & y, z
LUX_API void lxMeshSphere_getCounts(int segs[2], int* numVertices, int* numTriangleIndices, int* numOutlineIndices)
{
  *numVertices = (segs[0]+1) * (segs[1]+1);
  *numTriangleIndices = ((segs[0] * (segs[1]-2) * 2) + (segs[0] * 2)) * 3;
  *numOutlineIndices  = (segs[0] + (segs[1] * 4)) * 2;
}

LUX_API void lxMeshSphere_initTriangles(int segs[2], lxVector3* pos, lxVector3* normal, lxVector2* uv, uint32* indices)
{
  int xydim = segs[0];
  int zdim  = segs[1];

  float xyshift = 1.0f / (float)xydim;
  float zshift  = 1.0f / (float)zdim;
  int width = xydim + 1;

  int vertex = 0;
  int index = 0;
  int xy,z;
  for (z = 0; z < zdim + 1; z++){
    for (xy = 0; xy < xydim + 1; xy++, vertex++){
      float curxy = xyshift * (float)xy;
      float curz  = zshift  * (float)z;
      float anglexy = curxy * LUX_MUL_TWOPI;
      float anglez  = (1.0f-curz) * LUX_MUL_PI;
      pos[vertex][0] = cosf(anglexy) * sinf(anglez);
      pos[vertex][1] = sinf(anglexy) * sinf(anglez);
      pos[vertex][2] = cosf(anglez);
      lxVector3Copy(normal[vertex], pos[vertex]);
      uv[vertex][0]  = curxy;
      uv[vertex][1]  = curz;
    }
  }
  
  vertex = 0;
  for (z = 0; z < zdim; z++){
    for (xy = 0; xy < xydim; xy++, vertex++){
      if (z != zdim-1){
        indices[index++] = vertex;
        indices[index++] = vertex + width;
        indices[index++] = vertex + width + 1;
      }
      if (z != 0){
        indices[index++] = vertex + width + 1;
        indices[index++] = vertex + 1;
        indices[index++] = vertex;
      }
    }
    vertex++;
  }
}

LUX_API void lxMeshSphere_initOutline(int segs[2], uint32* indices)
{
  int xydim = segs[0];
  int zdim  = segs[1];

  int middlez = segs[1] / 2;
  
  int width = xydim + 1;
  int index = 0;
  // aequatorial line (xy)
  int i,xy,z;
  for (xy = 0; xy < xydim; xy++){
    indices[index++] = middlez * width + xy;
    indices[index++] = middlez * width + xy + 1;
  }

  for (i = 0; i < 4; i++){
    int x = (segs[0] * i) / 4;
    for (z = 0; z < zdim; z++){
      indices[index++] = x + width * (z);
      indices[index++] = x + width * (z + 1);
    }
  }

}

// segs = x,cap,z
LUX_API void lxMeshCylinder_getCounts(int segs[3], int* numVertices, int* numTriangleIndices, int* numOutlineIndices)
{
  int used[2];
  int numv;
  int numtris;
  int numline;

  used[0] = segs[0];
  used[1] = segs[1];
  lxMeshDisc_getCounts(used,&numv,&numtris,&numline);
  *numVertices = numv * 2;
  *numTriangleIndices = numtris * 2;
  *numOutlineIndices = numline * 2;

  used[0] = segs[0];
  used[1] = segs[2];
  lxMeshPlane_getCounts(used,&numv,&numtris,&numline);
  *numVertices += numv;
  *numTriangleIndices += numtris;
  *numOutlineIndices += segs[2]*4 * 2;

}

LUX_API void lxMeshCylinder_initTriangles(int segs[3], lxVector3* pos, lxVector3* normal, lxVector2* uv, uint32* indices)
{
  lxMatrix44 matrix;
  lxVector3 angles;
  int used[2];
  int numv;
  int numtris;
  int numline;

  int offsetv = 0;
  int offsettris = 0;
  int v;

  used[0] = segs[0];
  used[1] = segs[1];
  lxMeshDisc_getCounts(used,&numv,&numtris,&numline);

  lxMeshDisc_initTriangles(used,pos + offsetv, normal + offsetv, uv + offsetv, indices + offsettris);
  lxMatrix44Identity(matrix);
  lxMeshHelper_applyTransform(numv, pos + offsetv, normal + offsetv, matrix);
  lxMeshHelper_applyOffset(numtris, indices + offsettris, offsetv);
  offsetv += numv;
  offsettris += numtris;

  lxMeshDisc_initTriangles(used,pos + offsetv, normal + offsetv, uv + offsetv, indices + offsettris);
  lxVector3Set(angles,0,LUX_MUL_PI,0);
  lxMatrix44FromEulerXYZ(matrix,angles);
  lxMeshHelper_applyTransform(numv, pos + offsetv, normal + offsetv, matrix);
  lxMeshHelper_applyOffset(numtris, indices + offsettris, offsetv);
  offsetv += numv;
  offsettris += numtris;

  used[0] = segs[2];
  used[1] = segs[0];
  lxMeshPlane_getCounts(used,&numv,&numtris,&numline);
  lxMeshPlane_initTriangles(used,pos + offsetv, normal + offsetv, uv + offsetv, indices + offsettris);
  lxMeshHelper_applyOffset(numtris, indices + offsettris, offsetv);


  for (v = 0; v < numv; v++){
    // x becomes z
    float zpos = pos[offsetv + v][0];

    // deform using uv-y
    float angle = uv[offsetv + v][1] * LUX_MUL_TWOPI;
    float xpos = cosf(angle);
    float ypos = sinf(angle);
    
    pos[offsetv + v][0] = xpos;
    pos[offsetv + v][1] = ypos;
    pos[offsetv + v][2] = zpos;

    normal[offsetv + v][0] = xpos;
    normal[offsetv + v][1] = ypos;
    normal[offsetv + v][2] = 0.0f;
  }
}

LUX_API void lxMeshCylinder_initOutline(int segs[3], uint32* indices)
{
  int used[2];
  int numv;
  int numtris;
  int numline;

  int offsetv = 0;
  int offsetline = 0;

  int i,x;

  used[0] = segs[0];
  used[1] = segs[1];
  lxMeshDisc_getCounts(used,&numv,&numtris,&numline);
  lxMeshDisc_initOutline(used,indices + offsetline);
  offsetv    += numv;
  offsetline += numline;
  lxMeshDisc_initOutline(used,indices + offsetline);
  lxMeshHelper_applyOffset(numline, indices + offsetline, offsetv);
  offsetline += numline;
  offsetv    += numv;
  // let's add 4 z(x) lines
  {
    int index = offsetline;
    int width = segs[2] + 1;
    for (i = 0; i < 4; i++){
      int y = (segs[0] * i) / 4;
      for (x = 0; x < segs[2]; x++){
        indices[index++] = offsetv + y * width + x;
        indices[index++] = offsetv + y * width + x + 1;
      }
    }
  }
}

