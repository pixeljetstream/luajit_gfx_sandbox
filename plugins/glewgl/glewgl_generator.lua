-- copy original files
-- and strip __glew names
dofile("../_common/misc.lua")

local WRAPPED = true

local function funcwrapper(func,eol)
  local output = ""
  local returns = func.def:match("^void%s*%w") ~= nil and "" or "return "
  local args = func.def:match("(%b())")
  local funcargs = {}
  local callargs = {}
  local cnt = 0
  for arg in args:gmatch("[^,%(%)]+") do
    local arg = arg:gsub("^%s*",""):gsub("%s*$",""):gsub("%s+"," ")
    local var = arg:gsub("%b[]","")
    var = var == "void" and "" or var
    var = var:match("[_%w]+$")
    
    if (arg ~= "void" and (var == arg or not var)) then
      var = " _a"..cnt
      arg = arg..var
    end
    cnt = cnt + 1
    table.insert(funcargs,arg)
    table.insert(callargs,var)
  end
  
  funcargs = "("..table.concat(funcargs,",")..")"
  callargs = "("..table.concat(callargs,",")..")"
  
  local outfunc = func.def:gsub("(%b())",funcargs)
  local check = "GLEW_USER_ASSERT("..func.mangled..");"
  
  output = output..outfunc:sub(1,-2).."{"..eol..check..eol..returns..func.mangled..callargs..";}"..eol
  
  return output
end

local function processHeader(name,idir,odir,onlybinding)
  print("processing...",name)
  local inheader = io.open(idir.."include/GL/"..name,"rb")
  local outheader = (not onlybinding) and io.open(odir.."include/GL/"..name,"wb")
  if (onlybinding) then
    outheader = {
      write = function() end,
      close = function() end,
    }
  end
  
  local exts = {}
  local types = {}
  local funcs = {}
  local vars = {}
  local enums = {}
  local lkproc2name = {}
  local lkproc2func = {}
  
  local lktypes = {}
  local lkenumignore = {}
  local lkproc2type = {}
  local lkmangled2name = {}
  local lkenumvalue = {}
  local lkprocused = {}
  
  local record = nil
  local eol
  local first = true
  for l in FileLines(inheader) do
    local output = l
    if (first) then
      eol = l:match("[\r\n]+")
      outheader:write("#ifdef GLEW_MX"..eol)
      outheader:write('#pragma error "unsupported option"'..eol)
      outheader:write("#endif"..eol)
      outheader:write(eol)
      outheader:write("#ifndef GLEW_USER_ASSERT"..eol)
      outheader:write("#define GLEW_USER_ASSERT(condition)"..eol)
      outheader:write("#endif"..eol)
      first = false
    end
    
    local ext = l:match("%/%* [%-]+ ([_%w]+)")
    if (ext) then
      record = ext
    elseif (l:match("%/%* [%-]+ %*%/")) then
      record = nil
    elseif (l:match("#define [_%w]+ GLEW_GET_FUN")) then
      -- ignore "#define glAccum GLEW_GET_FUN(__glewAccum)"
      -- but track name
      local fn,mangled = l:match("#define ([_%w]+) GLEW_GET_FUN%(([_%w]+)%)")
      lkmangled2name[mangled] = fn
      
      output = ""
    elseif (l:match("#define GLEW[_%w]+ GLEW_GET_VAR")) then
      -- do nothing but this prevents record to enum
    elseif (l:match("^GLEW_FUN_EXPORT")) then
      -- rename __glewFunc to glFunc
      -- and log function
      
      local pfn,mangled = l:match("GLEW_FUN_EXPORT ([_%w]+) ([_%w]+)")
      local fn = lkmangled2name[mangled]

      if (pfn and lkproc2type[pfn]) then
        local typ = lkproc2type[pfn]
        lkprocused[pfn] = true
        lkproc2name[pfn] = fn
        
        local fndef = typ.str:sub(#("typedef ")+1,-1)
        fndef = fndef:gsub(pfn,fn)
        local def = fndef:gsub("%(%s*%*%s*",""):gsub("%)%s*%(","(")
        if (WRAPPED) then
          fndef = def
          output = "GLEWAPI "..def..eol
        else
          fndef = fndef:sub(1,-2)..' __asm__("'..mangled..'");'
        end
        local func = {str=fndef, def=def, name = fn, mangled=mangled, ext=typ.ext}
        lkproc2func[pfn] = func
        table.insert(funcs, func)
        if (not WRAPPED) then
          output = output.."__inline "..funcwrapper(func,eol)..eol
        end
      else
        output = "GLEW_FUN_EXPORT "..pfn.." "..fn..";"..eol
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
        table.insert(enums,{str=enum.." = "..value..",", name=enum, ext=record})
      end
      
    elseif (record and l:match("^typedef .+%([%w]+%s*%*"))then
      -- log functiontypes
      --  typedef void (GLAPIENTRY * PFNGLACCUMPROC) (GLenum op, GLfloat value);
      local api,pfn = l:match("^typedef [^%(]+ %(([%w]+)%s*%*%s*([_%w]+)%)")
      local fn  = l:match("(.+;)"):gsub(api,"")
      lkproc2type[pfn] = {str=fn, name=pfn, orig=l, ext=record}
      
    elseif (record and l:match("typedef ")) then
      -- due to different platform types, let's use the last definition
      local def = l:match("(.+;)")
      local key = l:match("([_%w]+);")
      if (not lktypes[key]) then
        lktypes[key] = true
        table.insert(types,{str=def, name=key, ext=record})
      end
    end
    
    outheader:write(output)
  end
  outheader:close()
  
  for pfn,def in pairs(lkproc2type) do
    if (not lkprocused[pfn]) then
      table.insert(types,def)
    end
  end
  
  local lkenumexported = {}
  for i,v in ipairs(enums) do
    local enum,value = v.str:match("([_%w]+) = ([_%w]+)")
    local ext = v.ext
    if (not lkenumexported[enum]) then
      lkenumexported[enum] = true
      local nval = lkenumvalue[value]
      local lval 
      while (nval) do
        lval = nval
        nval = lkenumvalue[nval]
      end
      if (lval) then
        enums[i] = {str = enum.." = "..lval..",", name = enum, ext=ext }
      end
    else
      enums[i] = {str ="//"..v.str, name = enum, ext=ext }
    end
  end
  
  return {
    funcs = funcs, 
    vars = vars, 
    types = types, 
    enums = enums, 
    eol = eol, 
    lkproc2func = lkproc2func,
    exts = exts
    }
end

local function copyFile(name,idir,odir)
  print("copying...",name)
  local infile = io.open(idir..name,"rb")
  local outfile = io.open(odir..name,"wb")
  
  for l in FileLines(infile) do
    outfile:write(l)
  end
  infile:close()
  outfile:close()
end

local function processSource(name,idir,odir,gl)
  print("processing...",name)
  local infile = io.open(idir.."src/"..name,"rb")
  local outfile = io.open(odir.."src/"..name,"wb")
  
  local eol
  local first = true
  for l in FileLines(infile) do
    eol = eol or l:match("[\r\n]+")
    
    local output = l
    if (l:match("[_%w]+PROC __[_%w]+ = NULL;")) then
      local pfn,mangled = l:match("([_%w]+PROC) (__[_%w]+) = NULL;")
      local func = gl.lkproc2func[pfn]
      if (func) then
        if (WRAPPED) then
          output = "static "..output
          output = output..funcwrapper(func,eol)
        end
      end
    elseif (l:match("%(%([_%w]+ = %([_%w]+PROC%)")) then
      --((glAccum = (PFNGLACCUMPROC)glewGetProcAddress
      output = l:gsub("%(%(([_%w]+) = %(([_%w]+PROC)%)", 
        function(fn,pfn)
          local func = gl.lkproc2func[pfn]
          if (func) then
            return "(("..func.mangled.." = ("..pfn..")"
          else
            return "(("..fn.." = ("..pfn..")"
          end
        end)
    end
    
    outfile:write(output or "")
  end
  outfile:close()
end



local function generateBinding(name,odir,gl,filter,ffiraw)
  local ofile = io.open(odir..name, "wb")
  local eol = gl.eol
  
  ofile:write("local ffi = require 'ffi'"..eol)
  ofile:write("ffi.cdef [["..eol)
  
  local lksymbols = {}
  for i,v in ipairs(gl.enums) do
    lksymbols[v.name] = true
  end
  for i,v in ipairs(gl.funcs) do
    lksymbols[v.name] = true
  end
  
  local filtered = function(id) return true end
  if (filter) then
    local except = {
      GLint64EXT = true,
      GLuint64EXT = true,
      -- funcs with different meaning
      glVertexAttribFormatNV = true,
      glVertexAttribIFormatNV = true,
      glVertexAttribLFormatNV = true,
    }
    local vendors = {}
    for i,ext in ipairs(gl.exts) do
      local vendor = ext:match("^%w+%_(%w+)%_")
      vendors[vendor or " "] = false
    end
    vendors.VERSION = nil
    
    local extensions = {}
    for cap in filter:gmatch("([%w_]+)%s*,?") do
      if (cap:find("_")) then
        extensions[cap] = true
      else
        vendors[cap] = true
      end
    end
    
    filtered = function (id,var,ext)
      if (not id or except[id] or not ext) then return true end
      
      if (var) then
        local extname = id:match("%_%_GLEW%_(%w+)%_")
        if (extname and extname ~= "VERSION") then
          return vendors[extname]
        end
      end
      
      local vendor = ext and ext:match("^%w+%_(%w+)%_")
      local baseid
      for i,v in pairs(vendors) do
        local length = #i
        if (id:sub(-length,-1) == i) then
          baseid = id:sub(1,-length-1)
          baseid = baseid:match("(.-)_?$")
          break
        end
      end
      
      local doexport = vendors[vendor]
      doexport = (doexport == nil) and true or doexport
      doexport = doexport or extensions[ext]
      
      -- prevent stuff that already is in core
      if (doexport and baseid and lksymbols[baseid]) then return false end
      
      return doexport
    end
  end
  
  for i,v in ipairs(gl.types) do
    local out = filtered(v.name,false,v.ext)
    ofile:write(out and v.str..eol or "")
  end
  ofile:write(eol)
  
  ofile:write("enum {"..eol)
  for i,v in ipairs(gl.enums) do
    local out = filtered(v.name,false,v.ext)
    ofile:write(out and v.str..eol or "")
  end
  ofile:write("};"..eol..eol)
  
  for i,v in ipairs(gl.funcs) do
    local out = filtered(v.name,false,v.ext)
    ofile:write(out and "extern "..v.str..eol or "")
  end
  ofile:write(eol)
  
  for i,v in ipairs(gl.vars) do
    local out = filtered(v.name,true,v.ext)
    ofile:write(out and "extern "..v.str..eol or "")
  end
  ofile:write("]]"..eol..eol)
  ofile:write("return ffi.load('glewgl')"..eol)
  
  ofile:close()
end

local function makebinding(name,idir,odir,filter,onlybinding)
  local gl = processHeader("glew.h", idir, odir,onlybinding)
  if (not onlybinding) then
    copyFile("include/GL/wglew.h", idir, odir)
    copyFile("include/GL/glxew.h", idir, odir)

    processSource("glew.c", idir, odir, gl)
  end

  table.insert(gl.funcs,{str="GLenum glewInit ();", name = "glewInit"})
  table.insert(gl.funcs,{str="GLboolean glewIsSupported (const char* name);", name = "glewIsSupported"})

  generateBinding(name..(filter and "_filtered" or "")..".lua",odir,gl,filter)
end

WRAPPED = true
makebinding(
  "glewgl",
  RELPATH ("glewinput/"),
  RELPATH (""),
  "ARB,NV,AMD,NVX,KHR,GL_EXT_direct_state_access",
  true)
  
WRAPPED = false
makebinding(
  "glewgl",
  RELPATH ("glewinput/"),
  RELPATH (""),
  nil,
  nil)


