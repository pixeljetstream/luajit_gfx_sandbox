dCmodule2ffi = {}

function dCmodule2ffi.new(class)
  local self = {
    funcs = {},
    types = {},
    structs = {},
    enums = {},
    consts = {},
  }
  
  
	setmetatable(self,{
		__index = class,})
  return self
end

function dCmodule2ffi:parse(txt)
  local txt = StripCommentsC(txt)
  
  txt = txt:match("extern%s*%(%s*C%s*%)%s*(%b{})")
  if (not txt) then return end
  
  txt = txt:sub(2,-2) 
  
  -- for functions and variables, get rid of "inner" definitions
  local outer = txt:gsub("(%b{})","{}")
  
  -- consts
  for const in outer:gmatch("const (.-);") do
  
  end
  
  -- funcs
  for ret,args,name in outer:gmatch("[_w%]+%s+function%((.-)%)%s*([_%w]+);") do
    table.insert(self.funcs, {ret=ret,args=args,name=name})
  end
  
  -- types
  for alias in outer:gmatch("alias (.-);") do
  
  end
  
  -- structs
  for name,def in txt:gmatch("struct%s+([_%w]+)%s*(%b{})") do

  end
  
  -- enums
  for name,def in txt:gmatch("enum%s+([_%w]+).-(%b{})") do
  
  end
  
end

function dCmodule2ffi:output()
  
end


