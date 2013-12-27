local glfw = require "glfw"
local ffi  = require "ffi"
local gl   = require "glewgl"

function THINK(time)
  local clearbits = bit.bor(gl.GL_COLOR_BUFFER_BIT,gl.GL_STENCIL_BUFFER_BIT,gl.GL_DEPTH_BUFFER_BIT)
  
  gl.glClearColor(0.2,0.2,0.2,0)
  gl.glClear(clearbits)
end
