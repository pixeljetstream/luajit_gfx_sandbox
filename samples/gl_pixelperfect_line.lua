-- Test an algorithm to draw pixel-perfect lines 

local ffi   = require "ffi"
local gl    = require "opengl"
local glfw  = require "glfw3"
local glapp = require "glfw3app"
local glu   = require "glutils"


local sscale = 8
local swidth,sheight = 128, 128
local wwidth,wheight = swidth*sscale, sheight*sscale


local app = glapp.init("test",wwidth,wheight)
print(app.vendor, app.renderer, app.driver)


local tex = glu.resource(gl.glCreateTextures, gl.glDeleteTextures, gl.GL_TEXTURE_2D)
gl.glTextureStorage2D(tex.handle, 1, gl.GL_RGBA8, swidth, sheight)

local fbo = glu.resource(gl.glCreateFramebuffers, gl.glDeleteFramebuffers)
gl.glNamedFramebufferTexture( fbo.handle, gl.GL_COLOR_ATTACHMENT0, tex.handle, 0)
gl.glDisable(gl.GL_DEPTH_TEST)

local function drawLine(a,b, color, viewport)
  gl.glBegin(gl.GL_LINES)
  gl.glColor4f(unpack(color))
  gl.glVertex2f(((a[1]+0.0)/viewport[1])*2 - 1, ((a[2]-0.0)/viewport[2])*2-1)
  gl.glVertex2f(((b[1]+0.0)/viewport[1])*2 - 1, ((b[2]-0.0)/viewport[2])*2-1)
  gl.glEnd()
end

local function angleSnap(pta, ptb)
  local sx = (pta[1])
  local sy = (pta[2])
  
  local ex = (ptb[1])
  local ey = (ptb[2])
  
  local dx = ex - sx
  local dy = ey - sy
  
  local adx = math.abs(dx)
  local ady = math.abs(dy)
  
  if (ady == 0 or adx == 0) then
    return {sx,sy},{ex,ey} 
  end

  -- reduce to 0-45 degree problem
  local flip = adx < ady
  if (flip) then
    dx,dy = dy,dx
  end
  
  local function normalize(x,y)
    local length = math.sqrt(x*x + y*y)
    return {x/length, y/length}
  end
  
  local function dot(a, b)
    return a[1]*b[1] + a[2]*b[2]
  end
  
  local dnorm = normalize(dx,dy)
  
  -- find closest division
  local div0  = math.floor(dx/dy)
  local div1  = math.ceil (dx/dy)
  local snx0  = div0 * dy
  local snx1  = div1 * dy
  
  -- find closest angle
  local a0    = dot(dnorm,normalize(snx0,dy))
  local a1    = dot(dnorm,normalize(snx1,dy))
  local div   = a0 > a1 and div0 or div1

  -- keep close to bounds
  local sny0 = math.ceil(dx/div)
  local sny1 = math.floor(dx/div)
  local sny = math.abs(sny0 * div - dx) < math.abs(sny1 * div - dx) and sny0 or sny1
  local snx = sny * div
  
  -- flip x/y back
  if (flip) then
    snx,sny = sny,snx
  end
  
  return {sx,sy},{sx + snx, sy + sny}
end


local mpos = ffi.new("double[2]")
local function think(width, height, frame, timeseconds, timedelta, timeavg)
  glfw.glfwGetCursorPos(app.window, mpos, mpos+1)
  
  local pta = {swidth/2,sheight/2}
  local ptb = {math.floor(mpos[0]/sscale), sheight - math.ceil(mpos[1]/sscale)}
  
  local ptas, ptbs = angleSnap(pta, ptb)
  
  gl.glBindFramebuffer(gl.GL_FRAMEBUFFER, fbo.handle)
  gl.glViewport(0,0,swidth,sheight)
  gl.glClearColor(0.2,0.2,0.2,0.2)
  gl.glClear(gl.GL_COLOR_BUFFER_BIT)
  
  gl.glLineWidth(1)
  drawLine(ptas,ptbs,{1,1,1,1},{swidth,sheight})
  
  gl.glBindFramebuffer(gl.GL_FRAMEBUFFER, 0)
  gl.glViewport(0,0,wwidth,wheight)
  gl.glBlitNamedFramebuffer(fbo.handle, 0,  0,0,swidth,sheight, 0,0, wwidth,wheight, gl.GL_COLOR_BUFFER_BIT, gl.GL_NEAREST)
    
  gl.glLineWidth(2)
  drawLine(pta,ptb,{1,0.4,0,0},{swidth,sheight})

end

glapp.loop(app, think, {timetitle=true})
