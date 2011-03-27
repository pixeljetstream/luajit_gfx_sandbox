-- copy original files
-- and strip __glew names

-- http://lua-users.org/wiki/EnhancedFileLines
local function FileLines(f)
  local CHUNK_SIZE = 1024
  local buffer = ""
  local pos_beg = 1
  return function()
    local pos, chars
    while 1 do
      pos, chars = buffer:match('()([\r\n].)', pos_beg)
      if pos or not f then
        break
      elseif f then
        local chunk = f:read(CHUNK_SIZE)
        if chunk then
          buffer = buffer:sub(pos_beg) .. chunk
          pos_beg = 1
        else
          f = nil
        end
      end
    end
    if not pos then
      pos = #buffer
    elseif chars == '\r\n' then
      pos = pos + 1
    end
    local line = buffer:sub(pos_beg, pos)
    pos_beg = pos + 1
    if #line > 0 then
      return line
    end    
  end
end


local function processHeader(name,idir,odir)
  print("processing...",name)
  local inheader = io.open(idir.."/include/GL/"..name,"rb")
  local outheader = io.open(odir.."/include/GL/"..name,"wb")
  
  local types = {}
  local funcs = {}
  local vars = {}
  local enums = {}
  local lkfuncnames = {}
  
  local lktypes = {}
  local lkenumignore = {}
  local lkfunctypes = {}
  local lkfuncmangled = {}
  local lkenumvalue = {}
  local lkfuncused = {}
  
  local record = false
  local eol
  local first = true
  for l in FileLines(inheader) do
    local output = l
    if (first) then
      eol = l:match("[\r\n]+")
      outheader:write("#ifdef GLEW_MX"..eol)
      outheader:write('#pragma error "unsupported option"'..eol)
      outheader:write("#endif"..eol)
      first = false
    end
    
    if (l:match("%/%* [%-]+ [_%w]+")) then
      record = true
    elseif (l:match("%/%* [%-]+ %*%/")) then
      record = false
    elseif (l:match("#define [_%w]+ GLEW_GET_FUN")) then
      -- ignore "#define glAccum GLEW_GET_FUN(__glewAccum)"
      -- but track name
      local fn,mangled = l:match("#define ([_%w]+) GLEW_GET_FUN%(([_%w]+)%)")
      lkfuncmangled[mangled] = fn
      
      output = ""
    elseif (l:match("#define GLEW[_%w]+ GLEW_GET_VAR")) then
      -- do nothing but this prevents record to enum
    elseif (l:match("^GLEW_FUN_EXPORT")) then
      -- rename __glewFunc to glFunc
      -- and log function
      
      local pfn,fn = l:match("GLEW_FUN_EXPORT ([_%w]+) ([_%w]+)")
      fn = lkfuncmangled[fn]
      output = "GLEW_FUN_EXPORT "..pfn.." "..fn..";"..eol

      if (pfn and lkfunctypes[pfn]) then
        lkfuncused[pfn] = true
        lkfuncnames[pfn] = fn
        local fndef = lkfunctypes[pfn]:sub(9,-1)
        local fndef = fndef:gsub(pfn,fn)
        table.insert(funcs, fndef)
      end
      
    elseif (l:match("GLEW_VAR_EXPORT GLboolean")) then
      -- log variables
      local var = l:match("GLEW_VAR_EXPORT GLboolean ([_%w]+);")
      table.insert(vars,"GLboolean "..var..";")

    elseif (record and l:match("#ifndef")) then
      -- ignore these when logging enums
      --  #ifndef GL_WIN_phong_shading
      --  #define GL_WIN_phong_shading 1
      
      local enum = l:match("#ifndef ([_%w]+)")
      lkenumignore[enum or " "] = true
      
    elseif (record and l:match("#define")) then
      -- log enums
      --  #define GL_PHONG_WIN 0x80EA
      --  #define GL_TEXTURE_COMPONENTS GL_TEXTURE_INTERNAL_FORMAT
      local enum,value = l:match("#define ([_%w]+) ([_%w]+)")
      if (enum and value and not lkenumignore[enum]) then
        lkenumvalue[enum] = value
        table.insert(enums,enum.." = "..value..",")
      end
      
    elseif (record and l:match("^typedef .+%([%w]+%s*%*"))then
      -- log functiontypes
      --  typedef void (GLAPIENTRY * PFNGLACCUMPROC) (GLenum op, GLfloat value);
      local api,pfn = l:match("^typedef [^%(]+ %(([%w]+)%s*%*%s*([_%w]+)%)")
      local fn  = l:match("(.+;)"):gsub(api,"")
      lkfunctypes[pfn] = fn
      
    elseif (record and l:match("typedef ")) then
      -- due to different platform types, let's use the last definition
      local def = l:match("(.+;)")
      local key = l:match("([_%w]+);")
      if (not lktypes[key]) then
        lktypes[key] = true
        table.insert(types,def)
      end
    end
    
    outheader:write(output)
  end
  
  for pfn,def in pairs(lkfunctypes) do
    if (not lkfuncused[pfn]) then
      table.insert(types,def)
    end
  end
  
  local lkenumexported = {}
  for i,v in ipairs(enums) do
    local enum,value = v:match("([_%w]+) = ([_%w]+)")
    if (not lkenumexported[enum]) then
      lkenumexported[enum] = true
      local nval = lkenumvalue[value]
      local lval 
      while (nval) do
        lval = nval
        nval = lkenumvalue[nval]
      end
      if (lval) then
        enums[i] = enum.." = "..lval..","
      end
    else
      enums[i] = "//"..v
    end
  end
  
  return {funcs=funcs, vars=vars, types=types, enums=enums, eol=eol, lkfuncnames=lkfuncnames}
end

local function processSource(name,idir,odir,gl)
  print("processing...",name)
  local infile = io.open(idir.."/src/"..name,"rb")
  local outfile = io.open(odir.."/src/"..name,"wb")
  
  local eol
  local first = true
  for l in FileLines(infile) do
    eol = eol or l:match("[\r\n]+")
    
    local output = l
    if (l:match("[_%w]+PROC __[_%w]+ = NULL;")) then
      local pfn,name = l:match("([_%w]+PROC) __[_%w]+ = NULL;")
      local outname = gl.lkfuncnames[pfn]
      if (outname) then
        output = pfn.." "..outname.." = NULL;"..eol
      end
    else
      output = l:gsub("__glewGetString","glGetString")
    end
    
    outfile:write(output or "")
  end
  
end

local function copyFile(name,idir,odir)
  print("copying...",name)
  local infile = io.open(idir..name,"rb")
  local outfile = io.open(odir..name,"wb")
  
  for l in FileLines(infile) do
    outfile:write(l)
  end
end

local function generateBinding(name,odir,gl)
  local ofile = io.open(odir..name, "wb")
  local eol = gl.eol
  
  ofile:write("local ffi = require 'ffi'"..eol)
  ofile:write("ffi.cdef [["..eol)
  
  for i,v in ipairs(gl.types) do
    ofile:write(v..eol)
  end
  ofile:write(eol)
  
  ofile:write("enum {"..eol)
  for i,v in ipairs(gl.enums) do
    ofile:write("  "..v..eol)
  end
  ofile:write("};"..eol..eol)
  
  for i,v in ipairs(gl.funcs) do
    ofile:write("extern "..v..eol)
  end
  ofile:write(eol)
  
  for i,v in ipairs(gl.vars) do
    ofile:write("extern "..v..eol)
  end
  ofile:write("]]"..eol..eol)
  
  ofile:write("local glew = ffi.load('glewgl')"..eol)
  ofile:write("glew.glewInit()"..eol..eol)
  ofile:write("return glew"..eol)
end

local function makebinding(name,idir,odir)
  local gl = processHeader("glew.h", idir, odir)
  copyFile("include/GL/wglew.h", idir, odir)
  copyFile("include/GL/glxew.h", idir, odir)

  processSource("glew.c", idir, odir, gl)


  table.insert(gl.funcs,"GLenum glewInit ();")
  table.insert(gl.funcs,"GLboolean glewIsSupported (const char* name);")

  generateBinding(name,odir,gl)
end


makebinding(
  "glewgl.lua",
  RELPATH ("glewinput/"),
  RELPATH (""))
