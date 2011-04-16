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
    lktype = {},
    lkfunc  = {},
    lkstruct = {},
  }
  
  table.insert(self.types,{name="uint",alias="unsigned int uint"})
  table.insert(self.types,{name="byte",alias="char byte"})
  table.insert(self.types,{name="ubyte",alias="unsigned char ubyte"})
  
  setmetatable(self,{
    __index = class,})
  return self
end

function d2ffi:parse(txt)
  local txt = StripCommentsC(txt)
  
  local mod = txt:match("module%s+([_%.%w]+)%s*;") or ""
  print("mod",mod)
  txt = txt:match("extern%s*%(%s*C%s*%)%s*(%b{})")
  
  if (not txt) then return end
  txt = txt:sub(2,-2) 
  
  local function getOrNew(tab)
    local t = tab[mod] or {}
    tab[mod] = t
    return t
  end
  
  
  local lkc = getOrNew(self.lkconst)
  local lke = getOrNew(self.lkenum)
  local lkf = getOrNew(self.lkfunc)
  local lkt = getOrNew(self.lktype)
  local lks = getOrNew(self.lkstruct)

  
  -- for functions and variables, get rid of "inner" definitions
  local outer = txt:gsub("(%b{})","{}")
  local lkvalue = {}
  
  -- enums
  for name,def in txt:gmatch("enum%s+([_%w]+).-(%b{})") do
    local data = {}
    local lastvalue = -1
    for var,value in def:gmatch("([_%w]+)%s*=?%s*(.-)[\r\n,}]") do
      print("enum",name,var,value)
      table.insert(data,{name=var,value=value})
    end
    local enum = {name=name,mod=mod,data=data}
    lke[name] = enum
    table.insert(self.enums,enum)
  end
  
  -- types
  outer = outer:gsub("alias (.-;)", function(alias)
    -- check for simple type or functionptr
    local name=alias:match("([_%w]+)%s*;")
    local alias,cnt=alias:gsub("function","(*"..name..")")
    if (cnt > 0) then
      alias=alias:gsub(name.."%s*;",";")
    end
    alias=alias:sub(1,-2)
    print("alias",name,alias)
    alias = {name=name, alias=alias}
    lkt[name] = alias
    table.insert(self.types,alias)
    return ""
  end)
  
  -- consts
  outer = outer:gsub("const%s+([_%*%w]-)%s*([_%w]+)%s*=%s*(.-)%s*;",
  function(typ,name,value)
    typ = (typ ~= "" and typ or "uint")
    print("const",typ,name,value)
    local const = {name=name,value=value,typ=typ,mod=mod}
    table.insert(self.consts, const)
    lkc[name] = const
    lkvalue[name] = value
    return ""
  end)
  
  -- funcs
  outer = outer:gsub("([_%*%w]+)%s+function%s*(%b())%s*([_%w]+)%s*;",
  function(ret,args,name)
    if (ret and name and args) then
      args = args:gsub("[\r\n]"," ")
      args = args:gsub("=.-[,)]",function(cap) if (cap:match(",")) then return "," else return ")" end end)
      args = args:gsub("%s+"," ")
      print("fn",ret,name,args)
      local func = {ret=ret,args=args,name=name,mod=mod}
      lkf[name] = func
      table.insert(self.funcs, func)
      return ""
    end
  end)
  
  -- remove align statement
  txt = txt:gsub("align%s*%b()%s*:","")
  
  -- structs
  for name,def in txt:gmatch("struct%s+([_%w]+)%s*(%b{})") do
    local data = {}
    for typ,var in def:gmatch("([_%*%w]+)%s+(.-)%s*;") do
      var =var:gsub("%[%s*([_%w]+)%s*%]",
        function(a) 
          local t = lkvalue[a]
          return t and "["..t.."]"
        end)
      print("struct",name,typ,var)
      typ = typ:gsub(name.."%s*%*","struct "..name.."*")
      table.insert(data,{name=var,typ=typ})
    end
    local struct = {name=name,mod=mod,data=data}
    lks[name] = struct
    table.insert(self.structs,struct)
  end
end

function d2ffi:prefixEnums()
  for i,v in ipairs(self.enums) do
    if (not v.prefixed) then
      for n,k in ipairs(v.data) do
        k.name = v.name.."_"..k.name
      end
    end
  end
end

function d2ffi:prefixData(tab,prefix)
  tab.prefixed = true
  for i,v in ipairs(tab.data) do
    v.name = prefix..v.name
  end
end


function d2ffi:output()
  local str = ""
  local function write(s)
    str = str..s.."\n"
  end
  
  for i,v in ipairs(self.enums) do
    if (not v.skip) then
      write("typedef enum "..v.name.." {")
      for n,k in ipairs(v.data) do
          write("  "..k.name..(k.value and (" = "..k.value) or "")..",")
      end
      write("} "..v.name..";")
    end
  end
  
  for i,v in ipairs(self.types) do
    if (not v.skip) then
      write("typedef "..v.alias..";")
    end
  end
  
  for i,v in ipairs(self.consts) do
    if (not v.skip) then
      write("static const "..v.typ.." "..v.name.." = "..v.value..";")
    end
  end
  
  for i,v in ipairs(self.structs) do
    if (not v.skip) then
      write("typedef struct "..v.name.." {")
      for n,k in ipairs(v.data) do
        write("  "..k.typ.." "..k.name..";")
      end
      write("} "..v.name..";")
    end
  end
  
  for i,v in ipairs(self.funcs) do
    if (not v.skip) then
      write(v.ret.." "..v.name..v.args..";")
    end
  end
  
  return str
end


