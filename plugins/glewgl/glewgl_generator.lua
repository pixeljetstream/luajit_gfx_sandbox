-- copy original files
-- and strip __glew names

local RELPATH = RELPATH
if not (RELPATH) then
  RELPATH = function(str) return str end
end

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
  
  local exts = {}
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
        local fndef = lkfunctypes[pfn].str:sub(9,-1)
        fndef = fndef:gsub(pfn,fn)
        table.insert(funcs, {str=fndef, name = fn})
      end
      
    elseif (l:match("GLEW_VAR_EXPORT GLboolean")) then
      -- log variables
      local var = l:match("GLEW_VAR_EXPORT GLboolean ([_%w]+);")
      table.insert(vars,{str="GLboolean "..var..";", name=var})

    elseif (record and l:match("#ifndef")) then
      -- ignore these when logging enums
      --  #ifndef GL_WIN_phong_shading
      --  #define GL_WIN_phong_shading 1
      
      local enum = l:match("#ifndef ([_%w]+)")
      lkenumignore[enum or " "] = true
      if (enum) then
        table.insert(exts,enum)
      end
    elseif (record and l:match("#define")) then
      -- log enums
      --  #define GL_PHONG_WIN 0x80EA
      --  #define GL_TEXTURE_COMPONENTS GL_TEXTURE_INTERNAL_FORMAT
      local enum,value = l:match("#define ([_%w]+) ([_%w]+)")
      if (enum and value and not lkenumignore[enum]) then
        lkenumvalue[enum] = value
        table.insert(enums,{str=enum.." = "..value..",", name=enum})
      end
      
    elseif (record and l:match("^typedef .+%([%w]+%s*%*"))then
      -- log functiontypes
      --  typedef void (GLAPIENTRY * PFNGLACCUMPROC) (GLenum op, GLfloat value);
      local api,pfn = l:match("^typedef [^%(]+ %(([%w]+)%s*%*%s*([_%w]+)%)")
      local fn  = l:match("(.+;)"):gsub(api,"")
      lkfunctypes[pfn] = {str=fn, name=pfn}
      
    elseif (record and l:match("typedef ")) then
      -- due to different platform types, let's use the last definition
      local def = l:match("(.+;)")
      local key = l:match("([_%w]+);")
      if (not lktypes[key]) then
        lktypes[key] = true
        table.insert(types,{str=def, name=key})
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
    local enum,value = v.str:match("([_%w]+) = ([_%w]+)")
    if (not lkenumexported[enum]) then
      lkenumexported[enum] = true
      local nval = lkenumvalue[value]
      local lval 
      while (nval) do
        lval = nval
        nval = lkenumvalue[nval]
      end
      if (lval) then
        enums[i] = {str = enum.." = "..lval..",", name = enum }
      end
    else
      enums[i] = {str ="//"..v.str, name = enum }
    end
  end
  
  return {
    funcs = funcs, 
    vars = vars, 
    types = types, 
    enums = enums, 
    eol = eol, 
    lkfuncnames = lkfuncnames, 
    exts = exts
    }
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

local function generateBinding(name,odir,gl,filter)
  local ofile = io.open(odir..name, "wb")
  local eol = gl.eol
  
  ofile:write("local ffi = require 'ffi'"..eol)
  ofile:write("ffi.cdef [["..eol)
  
  
  local filtered = function(id) return true end
  if (filter) then
    local except = {
      GLint64EXT = true,
      GLuint64EXT = true
    }
    local vendors = {}
    for i,ext in ipairs(gl.exts) do
      local vendor = ext:match("^%w+%_(%w+)%_")
      vendors[vendor or " "] = false
    end
    vendors.VERSION = nil
    
    for cap in filter:gmatch("([%w]+)%s*,?") do
      vendors[cap] = true
    end
    
    filtered = function (id,var)
      if (not id or except[id]) then return true end
      if (var) then
        local extname = id:match("%_%_GLEW%_(%w+)%_")
        if (extname and extname ~= "VERSION") then
          return vendors[extname]
        end
      end
      
      local suffix = id:match("[A-Z]+%s*$")
      return (not suffix) or vendors[suffix]~= nil and vendors[suffix]
    end
  end
  
  for i,v in ipairs(gl.types) do
    local out = filtered(v.name)
    ofile:write(out and v.str..eol or "")
  end
  ofile:write(eol)
  
  ofile:write("enum {"..eol)
  for i,v in ipairs(gl.enums) do
    local out = filtered(v.name)
    ofile:write(out and v.str..eol or "")
  end
  ofile:write("};"..eol..eol)
  
  for i,v in ipairs(gl.funcs) do
    local out = filtered(v.name)
    ofile:write(out and "extern "..v.str..eol or "")
  end
  ofile:write(eol)
  
  for i,v in ipairs(gl.vars) do
    local out = filtered(v.name,true)
    ofile:write(out and "extern "..v.str..eol or "")
  end
  ofile:write("]]"..eol..eol)
  
  ofile:write("local glew = ffi.load('glewgl')"..eol)
  ofile:write("glew.glewInit()"..eol..eol)
  ofile:write("return glew"..eol)
end

local function makebinding(name,idir,odir,filter)
  local gl = processHeader("glew.h", idir, odir)
  copyFile("include/GL/wglew.h", idir, odir)
  copyFile("include/GL/glxew.h", idir, odir)

  processSource("glew.c", idir, odir, gl)

  table.insert(gl.funcs,{str="GLenum glewInit ();", name = "glewInit"})
  table.insert(gl.funcs,{str="GLboolean glewIsSupported (const char* name);", name = "glewIsSupported"})

  generateBinding(name..".lua",odir,gl,filter)
  generateBinding(name.."_filtered.lua",odir,gl,filter)
end


makebinding(
  "glewgl",
  RELPATH ("glewinput/"),
  RELPATH (""),
  "NV")

copyFile("include/GL/glew.h",  RELPATH (""), RELPATH ("../../depend/"))
copyFile("include/GL/wglew.h", RELPATH (""), RELPATH ("../../depend/"))
copyFile("include/GL/glxew.h", RELPATH (""), RELPATH ("../../depend/"))