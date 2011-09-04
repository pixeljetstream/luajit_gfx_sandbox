local mj = require "mojoshader"

local fgen = {}
local fgenmeta = {__index=fgen}

function fgen:new()
  self = {}
  setmetatable(self,fgenmeta)
  self._replace = {}
  
  return self
end

-- only functions with API prefix will be added
function fgen:setApiPrefix(prefix)
  self._prefix = prefix
end

-- ignore code outside
function fgen:setContentPattern(pattern)
  self._pattern = pattern
end

-- search for string replacements
function fgen:setReplace(replace)
  self._replace = replace
end

function fgen:setPreProcessor(defines,includes)
  self._defins = defines
  self._includes = includes
end

function fgen:processFile(infile)
  local str = ""
  local f = io.open(infile,"rt")
  assert(f,"file not found:"..infile)
  local content = f:read("*a")
  f:close()

  if (self._pattern) then
    content = content:match(self._pattern) or ""
  end

  content = mj.preprocess(infile,content,self._defins) or ""

  -- remove function definitions
  content = content:gsub("([^;{}]+%b()%s*%b{})","")
  
  -- filter non-API functions
  local prefix = self._prefix
  content = content:gsub("([^;{}]+%b()%s*;)", 
    function(cap) 
      local rest,cnt = cap:gsub(prefix,"")
      if (cnt == 1) then
        return rest
      elseif (cap:find("typedef ")) then
        return cap
      else
        return ""
      end
    end)
    
  -- remove other prefixes
  content = content:gsub(prefix,"")
  
  
  return content
end

return fgen
