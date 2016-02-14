-- copyright 2015-2016 Christoph Kubisch
-- Released under MIT license


local ffi = require "ffi"

local _M = {}

function _M.m4identity(out)
  out[0] = 1
  out[1] = 0
  out[2] = 0
  out[3] = 0
  out[4] = 0
  out[5] = 1
  out[6] = 0
  out[7] = 0
  out[8] = 0
  out[9] = 0
  out[10] = 1
  out[11] = 0
  out[12] = 0
  out[13] = 0
  out[14] = 0
  out[15] = 1
  return out
end

function _M.m4tab()
  local m = {}
  _M.m4identity(m)
  return m
end

function _M.m4float()
  local m = ffi.new("float[16]")
  _M.m4identity(m)
  return m
end

function _M.m4copy(out,mat)
  for i=0,15 do
    out[i] = mat[i]
  end
  return out
end

function _M.m4scaled(out,x,y,z)
  out[0] = x
  out[5] = y
  out[10] = z
  return out
end

function _M.m4translated(out,x,y,z)
  out[12] = x
  out[13] = y
  out[14] = z
  return out
end

function _M.m4perspectiveGL(mat, fov, near,  far, aspect)
  local sine, cotangent, deltaZ;
  local radians = fov / 2.0 * math.pi / 180.0;

  deltaZ = far - near;
  sine = math.sin(radians);
  cotangent = math.cos(radians) / sine;

  _M.m4identity(mat);
  mat[0] = cotangent / aspect;
  mat[5] = cotangent;
  mat[10] = -(far + near) / deltaZ;
  mat[11] = -1.0;
  mat[14] = -2.0 * near * far / deltaZ;
  mat[15] = 0.0;
  return mat
end

function _M.m4perspectiveVK(mat, fov, near,  far, aspect)
  local sine, cotangent, deltaZ;
  local radians = fov / 2.0 * math.pi / 180.0;

  deltaZ = near - far;
  sine = math.sin(radians);
  cotangent = math.cos(radians) / sine;

  _M.m4identity(mat);
  mat[0] = cotangent / aspect;
  mat[5] = -cotangent;
  mat[10] = far / deltaZ;
  mat[11] = -1.0;
  mat[14] = near * far / deltaZ;
  mat[15] = 0.0;
  return mat
end

local function m4orthoGL(mat, left, right, bottom, top, near, far)
  _M.m4identity(mat);
  mat[0] = 2.0/(right-left);
  mat[5] = 2.0/(top-bottom);
  mat[10]= -2.0/(far-near);
  mat[12]= -(right + left)/(right-left);
  mat[13]= -(top + bottom)/(top-bottom);
  mat[14]= -(far + near)/(far - near);
  return mat
end

local function m4orthoVK(mat, left, right, bottom, top, near, far)
  _M.m4identity(mat);
  mat[0] = 2.0/(right-left);
  mat[5] = -2.0/(top-bottom);
  mat[10]= 1.0/(far-near);
  mat[12]= -(right + left)/(right-left);
  mat[13]= -(top + bottom)/(top-bottom);
  mat[14]= near/( near - far);
  return mat
end


function _M.m4orthoaspectGL(mat, height, near, far, aspect)
  local width = height * aspect;
  local right  = width * 0.5;
  local left = - right;
  local top = height * 0.5;
  local bottom = - top;

  m4orthoGL(mat, left, right, bottom, top, near, far)
  return mat
end

function _M.m4orthoaspectVK(mat, height, near, far, aspect)
  local width = height * aspect;
  local right  = width * 0.5;
  local left = - right;
  local top = height * 0.5;
  local bottom = - top;

  m4orthoVK(mat, left, right, bottom, top, near, far)
  return mat
end

_M.m4orthoGL = m4orthoGL
_M.m4orthoVK = m4orthoVK

function _M.m4mul(out,a,b)
  local clip = out
  local proj = a
  local modl = b
  
  clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
  clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
  clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
  clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

  clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
  clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
  clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
  clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

  clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
  clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
  clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
  clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

  clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
  clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
  clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
  clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
  return clip
end

function _M.m4mulA(a,b)
  local temp = _M.m4tab()
  _M.m4mul(temp,a,b)
  _M.m4copy(a,temp)
  return a
end

function _M.m4mulB(a,b)
  local temp = _M.m4tab()
  _M.m4mul(temp,a,b)
  _M.m4copy(b,temp)
  return b
end

function _M.m4mulaffine(out,a,b)
  local clip = out
  local proj = a
  local modl = b
  
  clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8]
  clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9]
  clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10]
  clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11]

  clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8]
  clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9]
  clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10]
  clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11]

  clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8]
  clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9]
  clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10]
  clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11]

  clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + proj[12];
  clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + proj[13];
  clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + proj[14];
  clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + proj[15];
  return clip
end

function _M.m4rotatedXYZ(m_mat, angles)
  local sxsy, cxsy;

  local cx = math.cos( angles[0] );
  local sx = math.sin( angles[0] );
  local cy = math.cos( angles[1] );
  local sy = math.sin( angles[1] );
  local cz = math.cos( angles[2] );
  local sz = math.sin( angles[2] );

  -- rot =  cy*cz          -cy*sz           sy
  --        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
  --       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

  m_mat[0] = ( cy*cz );
  m_mat[4] = ( -cy*sz );
  m_mat[8] = ( sy );

  sxsy = sx*sy;
  cxsy = cx*sy;

  m_mat[1] = ( sxsy*cz+cx*sz );
  m_mat[5] = ( cx*cz-sxsy*sz );
  m_mat[9] = ( -sx*cy );

  m_mat[2] = ( -cxsy*cz+sx*sz );
  m_mat[6] = ( sx*cz+cxsy*sz );
  m_mat[10] = ( cx*cy );
  
  return m_mat
end

function _M.m4lookat(out,from,to,upn)
  local m2 = {}
  local mat = {}
  local forward = {}
  local side = {}
  local up = {}

  forward[0] = to[0]-from[0];
  forward[1] = to[1]-from[1];
  forward[2] = to[2]-from[2];

  _M.v3normalize(forward,forward);

  _M.v3cross(side,forward,upn);
  _M.v3normalize(side);
  _M.v3cross(up,side,forward);
  _M.v3normalize(up);

  mat[0] = side[0];
  mat[4] = side[1];
  mat[8] = side[2];
  mat[12] = 0;

  mat[1] = up[0];
  mat[5] = up[1];
  mat[9] = up[2];
  mat[13] = 0;

  mat[2] = -forward[0];
  mat[6] = -forward[1];
  mat[10] = -forward[2];
  mat[14] = 0;

  mat[3] = 0;
  mat[7] = 0;
  mat[11] = 0;
  mat[15] = 1;

  _M.m4identity(m2);
  m2[12] = -from[0]
  m2[13] = -from[0]
  m2[14] = -from[0]
  _M.m4mul(out,mat,m2);
  
  return mat
end

function _M.v3tab(x,y,z)
  return {[0]=x,y,z}
end

function _M.v3float(x,y,z)
  return ffi.new("float[3]",x,y,z)
end

function _M.v3set(out,a,b)
  out[0] = a[0] * b[0]
  out[1] = a[1] * b[1]
  out[2] = a[2] * b[2]
  return out
end

function _M.v3mul(out,a,b)
  out[0] = a[0] * b[0]
  out[1] = a[1] * b[1]
  out[2] = a[2] * b[2]
  return out
end

function _M.v3add(out,a,b)
  out[0] = a[0] + b[0]
  out[1] = a[1] + b[1]
  out[2] = a[2] + b[2]
  return out
end

function _M.v3sub(out,a,b)
  out[0] = a[0] - b[0]
  out[1] = a[1] - b[1]
  out[2] = a[2] - b[2]
  return out
end

function _M.v3mad(out,a,b,c)
  out[0] = a[0] * b[0] + c[0]
  out[1] = a[1] * b[1] + c[1]
  out[2] = a[2] * b[2] + c[2]
  return out
end

function _M.v3scaledadd(out,a,b,c)
  out[0] = a[0] * b + c[0]
  out[1] = a[1] * b + c[1]
  out[2] = a[2] * b + c[2]
  return out
end

function _M.v3scale(out,a,b)
  out[0] = a[0] * b
  out[1] = a[1] * b
  out[2] = a[2] * b
  return out
end

function _M.v3neg(out,a)
  out[0] = -a[0]
  out[1] = -a[1]
  out[2] = -a[2]
  return out
end

function _M.v3dot(a,b)
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]
end

function _M.v3distance(a,b)
  vec = _M.v3tab()
  _M.v3sub(vec,a,b)
  return math.sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2])
end

function _M.v3length(vec)
  return math.sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2])
end

function _M.v3cross(out,a,b)
  out[0] = a[1] * b[2] - a[2] * b[1];
  out[1] = a[2] * b[0] - a[0] * b[2];
  out[2] = a[0] * b[1] - a[1] * b[0];
  return out
end

function _M.v3normalize(out,vec)
  local fracc = 1.0/math.sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2])
  out[0] = vec[0]*fracc
  out[1] = vec[1]*fracc
  out[2] = vec[2]*fracc
  return out
end

function _M.v3transform(out,mat,vec)
  out[0] = vec[0]*mat[0]+vec[1]*mat[4]+vec[2]*mat[8]+mat[12];
  out[1] = vec[0]*mat[1]+vec[1]*mat[5]+vec[2]*mat[9]+mat[13];
  out[2] = vec[0]*mat[2]+vec[1]*mat[6]+vec[2]*mat[10]+mat[14];
  return out
end

function _M.v4transform(out,mat,vec)
  out[0] = vec[0]*mat[0]+vec[1]*mat[4]+vec[2]*mat[8]+vec[3]*mat[12];
  out[1] = vec[0]*mat[1]+vec[1]*mat[5]+vec[2]*mat[9]+vec[3]*mat[13];
  out[2] = vec[0]*mat[2]+vec[1]*mat[6]+vec[2]*mat[10]+vec[3]*mat[14];
  out[3] = vec[0]*mat[3]+vec[1]*mat[7]+vec[2]*mat[11]+vec[3]*mat[15];
  return out
end


local namespaces = {
  m4 = {},
  v3 = {},
  v4 = {},
}

for i,v in pairs(_M) do
  local prefix, name = i:match("(..)(.*)")
  namespaces[prefix][name] = v
end

_M.namespaces = namespaces

return _M