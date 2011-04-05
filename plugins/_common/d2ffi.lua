d2ffi = {}

function d2ffi.new(class)
  local self = {
    funcs = {},
    types = {},
    structs = {},
    enums = {},
    consts = {},
    lkenum = {},
    lkconst = {},
  }
  
  
  setmetatable(self,{
    __index = class,})
  return self
end

function d2ffi:parse(txt)
  local txt = StripCommentsC(txt)
  
  local mod = txt:match("module%s+([_%.%w]+)%s+;") or ""
  txt = txt:match("extern%s*%(%s*C%s*%)%s*(%b{})")
  
  if (not txt) then return end
  txt = txt:sub(2,-2) 
  
  local lkc = self.lkconst[mod] or {}
  local lke = self.lkenum[mod] or {}
  self.lkconst[mod] = lkc
  self.lkenum[mod]  = lke
  
  -- for functions and variables, get rid of "inner" definitions
  local outer = txt:gsub("(%b{})","{}")
  local lkvalue = {}
  
  -- consts
  for typ,name,value in outer:gmatch("const%s+([_%*%w]-)%s*([_%w]+)%s*=%s*(.-)%s*;") do
    print("const",name,value)
    local const = {name=name,value=value,typ=(typ or "uint"),mod=mod}
    table.insert(self.consts, const)
    lkc[name] = const
    lkvalue[name] = value
  end
  
  -- funcs
  for ret,args,name in outer:gmatch("[_%*%w]+%s+function%s*(%b())%s*([_%w]+)%s*;") do
    if (reg and name and args) then
      args = args:gsub("[\r\n]"," ")
      print("fn",reg,name,args)
      table.insert(self.funcs, {ret=ret,args=args,name=name,mod=mod})
    end
  end
  
  -- types
  for alias in outer:gmatch("alias (.-;)") do
    -- check for simple type or functionptr
    local name=alias:match("([_%w]+)%s*;")
    local alias,cnt=alias:gsub("function","(*"..name..")")
    if (cnt > 0) then
      alias=alias:gsub(name.."%s*;",";")
    end
    alias=alias:sub(1,-2)
    print("alias",name,alias)
    table.insert(self.types,{name=name, alias=alias})
  end
  
  -- remove align statement
  txt = txt:gsub("align%s*%b()%s*:","")
  
  -- structs
  for struct,def in txt:gmatch("struct%s+([_%w]+)%s*(%b{})") do
    local data = {}
    for typ,name in def:gmatch("([_%*%w]+)%s+(.-)%s*;") do
      name = name:gsub("%[%s*([_%w]+)%s*%]",
        function(a) 
          local t = lkvalue[a]
          return t and "["..t.."]"
        end)
      print("struct",strname,typ,name)
      table.insert(data,{name=name,typ=typ})
    end
    table.insert(self.structs,{name=struct,mod=mod,data=data})
  end
  
  -- enums
  for enum,def in txt:gmatch("enum%s+([_%w]+).-(%b{})") do
    local data = {}
    for name,value in def:gmatch("([_%w]+)([^\r\n]+)[,}]") do
      print("enum",enum,name,value)
      table.insert(data,{name=name,value=value})
    end
    data = {name=enum,mod=mod,data=data}
    lke[enum] = data
    table.insert(self.enums,data)
  end
  
end

function d2ffi:output()
  local str = ""
  local function write(s)
    str = str..s.."\n"
  end
  
  for i,v in ipairs(self.types) do
    write("typedef "..v.alias..";")
  end

  for i,v in ipairs(self.enums) do
    write("enum "..v.name.." {")
    for n,k in ipairs(v.data) do
      write("  "..k.name..k.value..",")
    end
    write("};")
  end
  
  for i,v in ipairs(self.structs) do
    write("struct "..v.name.." {")
    for n,k in ipairs(v.data) do
      write("  "..k.typ.." "..k.name..";")
    end
    write("};")
  end
  
  for i,v in ipairs(self.funcs) do
    write(v.ret.." "..v.name..v.args..";")
  end
  
  return str
end


