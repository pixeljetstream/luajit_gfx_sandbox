local ffi   = require "ffi"
local gl    = require "opengl"
local glfw  = require "glfw3"
local bit   = require "bit"

local glapp = {}


function glapp.assert(test,msg)
  if (not (test)) then
    local tr = debug.traceback("assertion failed: "..msg,2)
    print(tr)
    glfw.glfwTerminate()
    os.exit(false)
  end
end

function glapp.exit(arg)
  glfw.glfwTerminate()
  os.exit(arg)
end

local function setupMisc()
  if (ffi.os == "Windows") then
    ffi.cdef [[
    void* GetCurrentProcess();
    int SetProcessAffinityMask( void* hdl, size_t mask);
    int GetProcessAffinityMask( void* hdl, size_t* procmask, size_t* sysmask);
    ]]
    
    local proc = ffi.C.GetCurrentProcess();
    local procmask = ffi.new("size_t[1]")
    local sysmask = ffi.new("size_t[1]")
    -- pin to one physical cpu for smoother timings
    ffi.C.GetProcessAffinityMask(proc,procmask,sysmask);
    if ( bit.band(tonumber(sysmask[0]),8) ~= 0) then
      procmask[0] = 8;
    elseif ( bit.band(tonumber(sysmask[0]),2) ~= 0) then
      procmask[0] = 2;
    end
    ffi.C.SetProcessAffinityMask(proc,procmask[0]);
  end
  io.stdout:setvbuf('no')
end

function glapp.init(title, width, height, properties)
  local properties = properties or {}
  
  local errfunc = ffi.cast("GLFWerrorfun",function(err,description)
    print(description)
  end)
  
  glfw.glfwSetErrorCallback(errfunc);

  if (glfw.glfwInit() ~= 1 ) then
    os.exit(-1)
  end

  if properties.context then
    glfw.glfwWindowHint(glfw.GLFW_CONTEXT_VERSION_MAJOR, properties.context[1]);
    glfw.glfwWindowHint(glfw.GLFW_CONTEXT_VERSION_MINOR, properties.context[2] or 0);
  end
  if properties.msaa then
    glfw.glfwOpenWindowHint(glfw.GLFW_FSAA_SAMPLES, properties.msaa)
  end
  
  local window = glfw.glfwCreateWindow(width, height, title, nil, nil);
  if (window == nil) then
    glapp.exit(-1)
  end

  glfw.glfwSetInputMode(window, glfw.GLFW_STICKY_KEYS, glfw.GLFW_TRUE)

  glfw.glfwMakeContextCurrent(window);
  glfw.glfwSwapInterval( properties.interval or 1);

  setupMisc()
  if (ffi.os == "Windows") then
    gl.glewInit()
  end
  
  gl.glViewport(0, 0, width, height);
  gl.glClearColor(0.2,0.2,0.2,0)
  gl.glClear( bit.bor(gl.GL_COLOR_BUFFER_BIT,gl.GL_STENCIL_BUFFER_BIT,gl.GL_DEPTH_BUFFER_BIT) )
  
  
  
  local trackkeys = {}
  local app = {
    title    = title,
    window   = window,
    renderer = ffi.string(gl.glGetString(gl.GL_RENDERER)),
    vendor   = ffi.string(gl.glGetString(gl.GL_VENDOR)),
    driver   = nil,
    trackkeys = trackkeys,
  }
  
  function app.pressed(key)
    local laststate = trackkeys[key] or glfw.GLFW_RELEASE
    trackkeys[key] = laststate
    return laststate == glfw.GLFW_RELEASE and glfw.glfwGetKey(window,key) == glfw.GLFW_PRESS
  end

  if (app.vendor:match("NVIDIA")) then
    pcall(function()
      local nvapi = require "nvapi"
      local ver   = ffi.new("uint32_t[1]")
      local drv   = ffi.new("NvAPI_ShortString")
      nvapi.NvAPI_SYS_GetDriverAndBranchVersion(ver,drv)
      app.driver = ffi.string(drv).." "..tonumber(ver[0])/100
      end)
  end
  
  return app
end


function glapp.exit(code)
  glfw.glfwTerminate()
  os.exit(code)
end


function glapp.loop(app, fnThink, properties)
  local sizes = ffi.new("int[2]")
  local props = properties or {}
  local window = app.window
  
  props.avgcnt = props.avgcnt or 128  
  local tavgs = ffi.new("double[?]", props.avgcnt)
  local frame = 0
  
  local tlast = glfw.glfwGetTime()
  while ( glfw.glfwWindowShouldClose(window) ~= glfw.GLFW_TRUE and glfw.glfwGetKey(window,glfw.GLFW_KEY_ESCAPE) ~= glfw.GLFW_PRESS ) 
  do
    local t = glfw.glfwGetTime()
    local tdelta = t - tlast
    tlast = t
    
    tavgs[frame % props.avgcnt] = tdelta
    local avgfrom = frame < 2 and 2 or 0
    local avgto   = math.min(frame,props.avgcnt-1)
    local avgcnt  = avgto-avgfrom+1
    local tavg = 0
    for i=avgfrom,avgto do
      tavg = tavg + tavgs[i]
    end
    tavg = avgcnt > 0 and tavg/(avgcnt) or tdelta
    
    if (props.timetitle) then
      glfw.glfwSetWindowTitle(window, string.format(app.title.." - %.3f ms, avg %.3f",tdelta, tavg) )
    end
    
    glfw.glfwGetFramebufferSize(window, sizes, sizes + 1);
    local width  = sizes[0]
    local height = sizes[1]

    fnThink(width, height, frame, t, tdelta, tavg)

    if (not props.noswap) then
      glfw.glfwSwapBuffers(window)
    end
    
    for i,v in pairs(app.trackkeys) do
      app.trackkeys[i] = glfw.glfwGetKey(window,i)
    end
    glfw.glfwPollEvents();
    
    frame = frame + 1
  end

  glfw.glfwDestroyWindow(window);

  glfw.glfwTerminate();
  os.exit(0);
end

return glapp