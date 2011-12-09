print "glfw-minimal-host"

local glfw = require "glfw"
local ffi  = require "ffi"
local gl   = require "glewgl"
local svPoll = LUXGLOBAL.svPoll

local function main()
  assert( glfw.glfwInit() )
  local window = glfw.glfwOpenWindow( 640, 480, glfw.GLFW_WINDOWED, "glfw-minimal-host", nil)
  assert( window )
  gl.glewInit()
  gl.glClearColor(0.2,0.2,0.2,0)
  local clearbits = bit.bor(bit.bor(gl.GL_COLOR_BUFFER_BIT,gl.GL_STENCIL_BUFFER_BIT),gl.GL_DEPTH_BUFFER_BIT)

  glfw.glfwEnable(window, glfw.GLFW_STICKY_KEYS);
  glfw.glfwSwapInterval(1);
  while glfw.glfwIsWindow(window) ~= 0 and glfw.glfwGetKey(window, glfw.GLFW_KEY_ESCAPE) ~= glfw.GLFW_PRESS 
  do
    local t = glfw.glfwGetTime()
    if( THINK ) then THINK(t) end
    gl.glClear(clearbits)

    glfw.glfwSwapBuffers()
    glfw.glfwPollEvents()
    svPoll()
  end
  glfw.glfwTerminate()
  os.exit(0)
end

main()