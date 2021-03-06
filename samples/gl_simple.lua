local ffi   = require "ffi"
local gl    = require "opengl"
local glfw  = require "glfw3"
local glapp = require "glfw3app"

local app = glapp.init("test",800,600)
print(app.vendor, app.renderer, app.driver)

local timescale = 1

local function think(width, height, frame, timeseconds, timedelta, timeavg)
  gl.glClearColor(math.sin(timeseconds*timescale) * 0.5 + 0.5,math.cos(timeseconds*timescale) * 0.5 + 0.5,0,0)
  gl.glClear( bit.bor(gl.GL_COLOR_BUFFER_BIT,gl.GL_STENCIL_BUFFER_BIT,gl.GL_DEPTH_BUFFER_BIT) )
  if ( app.pressed(glfw.GLFW_KEY_UP) ) then
    timescale = timescale * 2
  elseif ( app.pressed(glfw.GLFW_KEY_DOWN) ) then
    timescale = timescale * 0.5
  end
end

glapp.loop(app, think, {timetitle=true})
