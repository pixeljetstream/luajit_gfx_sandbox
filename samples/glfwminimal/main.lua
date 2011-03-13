print "hello"

local glfw = require "glfw"
local ffi = require "ffi"

local function main()
   assert( glfw.glfwInit() )
   local window = glfw.glfwOpenWindow( 640, 480, glfw.GLFW_WINDOWED, "Spinning Triangle", nil)
   assert( window )
   glfw.glfwEnable(window, glfw.GLFW_STICKY_KEYS);
   glfw.glfwSwapInterval(1);
   while glfw.glfwIsWindow(window) and glfw.glfwGetKey(window, glfw.GLFW_KEY_ESCAPE) ~= glfw.GLFW_PRESS 
   do
      local double t = glfw.glfwGetTime()
      
      glfw.glfwSwapBuffers()
      glfw.glfwPollEvents()
      LXG.svPoll()
   end
   glfw.glfwTerminate()
   os.exit(0)
end

main()