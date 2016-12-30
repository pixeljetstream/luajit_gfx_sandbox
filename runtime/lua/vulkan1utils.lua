local vkheaders = require "vulkan1header"
local vk  = require "vulkan1"
local ffi = require "ffi"

local vku = {
VK_LOD_CLAMP_NONE = 1000.0,
VK_WHOLE_SIZE     = 0xFFFFFFFFFFFFFFFFULL,
}

local structEnums = {}
local structTypes = vkheaders.core:match("VkStructureType {(.-)}")

structTypes:gsub("VK_STRUCTURE_TYPE_([%w_]+)", function(cap) 
  local name = "Vk"
  local isExt = {
    KHR = true, 
    EXT = true, 
    NV  = true, 
    NVX = true, 
    AMD = true,
    IMG = true,
  }
  cap:gsub("(%w+)",function(word) 
      name = name..(isExt[word] and word or (word:sub(1,1)..word:lower():sub(2,-1)))
  end)
  structEnums[name] = vk["VK_STRUCTURE_TYPE_"..cap]
end)

function vku.getStructEnum(name)
  return structEnums[name]
end

function vku.newStruct(name)
  local str = ffi.new(name, 0)
  str.sType = structEnums[name] or 0
  
  return str
end

function vku.fillStruct(ref)
  local typ = tostring(ref):match("cdata<struct ([%w_]+)")
  assert(typ)
  local enum = structEnums[typ]
  if (enum) then
    ffi.fill(ref, ffi.sizeof(typ))
    ref.sType = enum
  end
end

function vku.loadExtensions(getfunc, pfnstring)
  local extstring = (pfnstring or vkheaders.gsubplatforms(vkheaders.extensions, ""))
    :gsub("VKAPI_PTR", "VKAPI_CALL")
    :gsub("VKAPI_CALL",ffi.os == "Windows" and "__stdcall" or "")
    :gsub("VKAPI_ATTR", "")
  
  local extfuncs = {}
  for name in extstring:gmatch("%*PFN_(vk[%w_]+)") do
    local fn = ffi.cast("PFN_"..name, getfunc(name))
    if (fn ~= nil) then
      extfuncs[name] = fn
    end
  end
  return extfuncs
end

return vku