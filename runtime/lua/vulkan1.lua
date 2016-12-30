local vkheaders = require "vulkan1header"

local ffi = require("ffi")

local defcore       = vkheaders.cleanup( vkheaders.gsubplatforms(vkheaders.core, "") )
local defextensions = vkheaders.cleanup( vkheaders.gsubplatforms(vkheaders.extensions, "") )

ffi.cdef (defcore)
ffi.cdef (defextensions)

local vk = ffi.load("vulkan-1")

return vk
