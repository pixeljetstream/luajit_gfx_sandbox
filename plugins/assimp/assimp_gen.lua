dofile("../_common/misc.lua")
dofile("../_common/d2ffi.lua")

local bind = d2ffi:new()

bind:parse(io.open(RELPATH "dAssimp/versionInfo.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/types.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/math.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/config.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/fileIO.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/light.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/camera.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/animation.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/mesh.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/material.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/texture.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/scene.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/postprocess.d","rt"):read("*a"))
bind:parse(io.open(RELPATH "dAssimp/api.d","rt"):read("*a"))

bind.lkconst["assimp.config"]["PP_ICL_PTCACHE_SIZE"].skip = true
bind.lktype["assimp.api"]["aiGetMaterialFloat"].skip = true
bind.lktype["assimp.api"]["aiGetMaterialInteger"].skip = true
bind.lktype["assimp.postprocess"]["aiProcess"].skip = true

bind.lkconst["assimp.types"]["MAXLEN"].name = "AI_TYPES_MAXLEN"

for i,v in ipairs(bind.consts) do
  if (tonumber(v.value) == nil) then 
    v.skip = true
  end
end

for i,v in ipairs(bind.types) do
  v.alias = v.alias:gsub("aiFileIO%*","struct aiFileIO*")
  v.alias = v.alias:gsub("aiFile%*","struct aiFile*")
end

bind:prefixData(bind.lkenum["assimp.postprocess"]["aiPostProcessSteps"],"aiProcess_")
bind:prefixData(bind.lkenum["assimp.material"]["aiPropertyTypeInfo"],"aiPTI_")

bind:prefixEnums()
local output = bind:output()

if (true) then
  local f = io.open( RELPATH "assimp.lua", "wt")
  f:write("local ffi = require 'ffi'\n")
  f:write("ffi.cdef [[\n")
  f:write("// assimp ai | AssetImporter Model Loader Library\n")
  f:write(output)
  f:write("]]\n\n")
  f:write("return ffi.load('assimp')\n\n")
  f:close()
end

