local vk    = require "vulkan1"
local vku   = require "vulkan1utils"
local vkh   = require "vulkan1header"
local shdc  = require "shaderc"
local ffi   = require "ffi"
local gl    = require "opengl"
local glfw  = require "glfw3"
local glapp = require "glfw3app"


local function makeApi()
  local f = io.open("vulkan1api.lua", "wt")
  local def = vkh.cleanup( vkh.gsubplatforms(vkh.core, "") )..
              vkh.cleanup( vkh.gsubplatforms(vkh.extensions, "") )
  f:write("--[[")
  f:write(def:gsub("__stdcall",""))
  f:write("]]")
  f:close()
end
--makeApi()

local test = vku.newStruct("VkBufferMemoryBarrier")
print(test.sType, test.pNext)

ffi.cdef "typedef struct PackTest { VkMemoryAllocateInfo allocInfo; VkSubmitInfo submit; } PackTest;"
local reftest = ffi.new("PackTest")
vku.fillStruct(reftest.submit)
assert(reftest.submit.sType == vk.VK_STRUCTURE_TYPE_SUBMIT_INFO)

local extfuncs = vku.loadExtensions(
  function(name) 
    --return ffi.cast("void*",0)
    return string.len(name) 
  end)
print(extfuncs.vkRegisterObjectsNVX)

local app = glapp.init("test",800,600)
print(app.vendor, app.renderer, app.driver)

local timescale = 1

local function think(width, height, frame, timeseconds, timedelta, timeavg)
  gl.glClearColor(math.sin(timeseconds*timescale),math.cos(timeseconds*timescale),0,0)
  gl.glClear( bit.bor(gl.GL_COLOR_BUFFER_BIT,gl.GL_STENCIL_BUFFER_BIT,gl.GL_DEPTH_BUFFER_BIT) )
  if ( app.pressed(glfw.GLFW_KEY_UP) ) then
    timescale = timescale * 2
  elseif ( app.pressed(glfw.GLFW_KEY_DOWN) ) then
    timescale = timescale * 0.5
  end
end

glapp.loop(app, think, {timetitle=true})
