do
  local info = debug.getinfo(1,"S")
  local path = info.source:gsub("\\","/")
  path = path:match("@(.+)/.+$")
  local p = path .. "/bin_"..jit.os.."_"..jit.arch.."/clibs"
  local l = path .. "/lua"
  local s = jit.os == "Windows" and ".dll" or ".so"
  local cstr = string.format(";%s/?%s;%s/?51%s;%s/?/?%s;%s/?/?51%s;",p,s,p,s,p,s,p,s)
  local lstr = string.format(";%s/?.lua;%s/?/?.lua;%s/?/init.lua;",l,l,l)
  package.path  = package.path..lstr
  package.cpath = package.cpath..cstr
end