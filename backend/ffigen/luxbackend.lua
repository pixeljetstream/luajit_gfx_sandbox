local fgen = loadfile("ffigen.lua")()

local gen = fgen:new()
do
  gen:setApiPrefix("LUX_API%s+")
  gen:setContentPattern([[
#ifdef __cplusplus
extern "C"{
#endif
(.*)
#ifdef __cplusplus
}
#endif
]])
  gen:setPreProcessor({
    "__MSC__=1400",
    "LUX_COMPILER_MSC=1",
    "LUX_RESTRICT",
    "LUX_ALIGNSIMD_BEGIN=__declspec(align(16))",
    "LUX_ALIGNSIMD_END",
    "LUX_FASTCALL=__fastcall",})
--[[
  gen:setReplace({
    LUX_RESTRICT = "",
    LUX_ALIGNSIMD_BEGIN = "__declspec(align(16))", 
    LUX_ALIGNSIMD_END = "", 
    LUX_FASTCALL = "__fastcall"})]]
end
local p = RELPATH "../include/luxinia/"
local pout = RELPATH "../../runtime/lua/luxinia2/"

local function append(str,file)
  return str..gen:processFile(p..file)
end

local function export(lib,content,file,required)
  local of = io.open(pout..file,"wb")
  required = required or ""
  content = [[local ffi = require "ffi"
]]..required..[=[

ffi.cdef [[
// ]=]..lib..[[

]]..content..[=[
]]

return ffi.load("luxbackend")
]=]

  of:write(content)
  of:close()
end

local luxplatform = true
local luxmath  = true
local luxcore  = true
local luxgfx   = true
local luxscene = true


if(luxplatform)then
  local content = ""
  content = append(content,"luxplatform/luxtypes.h")
  
  export(
    "lxp | Lux Platform",
    content,
    "luxplatform.lua")

end

if(luxmath) then
  local content = ""
  content = append(content,"luxmath/basetypes.h")
  content = append(content,"luxmath/bounding.h")
  content = append(content,"luxmath/frustum.h")
  content = append(content,"luxmath/matrix44.h")
  content = append(content,"luxmath/float16.h")
  
  export(
    "lxm | Lux Math",
    content,
    "luxmath.lua",[[
require "luxinia2.luxplatform"
]])

end

if(luxgfx) then
  local content = ""
  content = append(content,"luxgfx/gfxtypes.h")
  content = append(content,"luxgfx/glconfig.h")
  content = append(content,"luxgfx/buffer.h")
  content = append(content,"luxgfx/vertex.h")
  content = append(content,"luxgfx/texture.h")
  content = append(content,"luxgfx/state.h")
  content = append(content,"luxgfx/rendertarget.h")
  content = append(content,"luxgfx/program.h")
  content = append(content,"luxgfx/context.h")
  
  export(
    "lxg | Lux Graphics",
    content,
    "luxgfx.lua",[[
require "luxinia2.luxplatform"
require "glewgl"
]])

end

if(luxcore) then
  local content = ""
  content = append(content,"luxcore/memorybase.h")
  content = append(content,"luxcore/contvector.h")
  content = append(content,"luxcore/contbitarray.h")
  content = append(content,"luxcore/conthash.h")
  content = append(content,"luxcore/contmap.h")
  content = append(content,"luxcore/contstringmap.h")
  
  content = append(content,"luxcore/memorygeneric.h")
  content = append(content,"luxcore/memorylist.h")
  content = append(content,"luxcore/memorypool.h")
  content = append(content,"luxcore/memorystack.h")
  content = append(content,"luxcore/sortradix.h")
  content = append(content,"luxcore/handlesys.h")
  content = append(content,"luxcore/refsys.h")
  
  export(
    "lxc | Lux Core",
    content,
    "luxcore.lua",[[
require "luxinia2.luxplatform"
]])

end

if(luxscene) then
  local content = ""
  content = append(content,"luxscene/meshbase.h")
  content = append(content,"luxscene/meshvcacheopt.h")
  --content = append(content,"luxscene/shader.h")
  --content = append(content,"luxscene/drawsystem.h")
  
  export(
    "lxs | Lux Scene",
    content,
    "luxscene.lua",[[
require "luxinia2.luxplatform"
require "luxinia2.luxmath"
require "luxinia2.luxcore"
require "luxinia2.luxgfx"
]])

end


