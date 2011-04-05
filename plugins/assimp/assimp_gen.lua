dofile("../_common/misc.lua")
dofile("../_common/dCmodule2ffi.lua")

local parser = dCmodule2ffi:new()
--[[
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/versionInfo.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/types.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/math.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/config.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/fileIO.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/light.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/camera.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/animation.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/mesh.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/material.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/texture.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/scene.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/postprocess.d","rt"):read("*a"))
dCmodule2ffi:parse(io.open(RELPATH "dAssimp/api.d","rt"):read("*a"))
]]
local output = dCmodule2ffi:output()
print(output)

if (false) then
  local f = io.open( RELPATH "assimp.lua", "wt")
  f:write(output)
  f:close()
end

