local vkheaders = require "vulkan1header"

local ffi = require("ffi")
local bit = require("bit")

local function cleanup(str)
return str 
:gsub("VKAPI_PTR", "VKAPI_CALL")
:gsub("VKAPI_CALL",ffi.os == "Windows" and "__stdcall" or "")
:gsub("VKAPI_ATTR", "")
:gsub("#[^\r\n]+", "")
:gsub("VK_DEFINE_HANDLE(%b())",                  function(cap) return "typedef struct "..cap:sub(2,-2).."_T* "..cap:sub(2,-2)..";" end)
:gsub("VK_DEFINE_NON_DISPATCHABLE_HANDLE(%b())", function(cap) return "typedef uint64_t "..cap:sub(2,-2)..";" end)
:gsub("VK_MAKE_VERSION(%b())",                   
  function(cap) local major,minor,patch = cap:match("(%d+)%s*,%s*(%d+)%s*,%s*(%d+)"); 
  return bit.bor( bit.lshift(tonumber(major),22), bit.lshift(tonumber(minor),12), tonumber(patch)) end)
end

local defcore = cleanup( vkheaders.gsubplatforms(vkheaders.core, ""))
local defextensions = cleanup(vkheaders.gsubplatforms(vkheaders.extensions, ""))

--[[
#define VK_MAKE_VERSION(major, minor, patch) \
    ((major << 22) | (minor << 12) | patch)

// Vulkan API version supported by this file
#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
#define VK_DEFINE_NON_DISPATCHABLE_HANDLE(object) typedef uint64_t object;

//static const float VK_LOD_CLAMP_NONE = 1000.0f;
//static const uint64_t VK_WHOLE_SIZE  = (~0ULL);
]]

ffi.cdef (defcore)
ffi.cdef (defextensions)

local vk = ffi.load("vulkan-1")

return vk
