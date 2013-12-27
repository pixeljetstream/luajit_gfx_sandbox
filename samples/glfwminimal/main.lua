local lxpath = (os.getenv("LUXINIA2") and os.getenv("LUXINIA2").."/..") or "../../runtime"
dofile(lxpath.."/setup_package_paths.lua")

print "glfw minimal"

local glfw = require "glfw"
local ffi  = require "ffi"
local gl   = require "glewgl"

dofile "scratch.lua"

local function main()
  local run = RUNNING
  assert( glfw.glfwInit(), "glfw init" )
  local status = glfw.glfwOpenWindow( 640, 480, 8,8,8,8,24,8, glfw.GLFW_WINDOW)
  assert( status == glfw.GLFW_TRUE, "glfw win open" )
  glfw.glfwSetWindowTitle( "glfw minimal")
  assert( gl.glewInit() == 0, "glew init")


  glfw.glfwEnable(glfw.GLFW_STICKY_KEYS)
  glfw.glfwDisable(glfw.GLFW_AUTO_POLL_EVENTS)
  glfw.glfwSwapInterval(1)

  while glfw.glfwGetWindowParam( glfw.GLFW_OPENED ) ~= 0 and glfw.glfwGetKey(glfw.GLFW_KEY_ESC) ~= glfw.GLFW_PRESS 
  do
    local t = glfw.glfwGetTime()
    
    THINK(t) end
    
    glfw.glfwSwapBuffers()
    glfw.glfwPollEvents()
  end
  
  glfw.glfwTerminate()
end

main()
