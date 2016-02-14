-- copyright 2009-2016 Christoph Kubisch
-- Released under MIT license

local csv = {}
local CSVFile = {}
csv.CSVFile = CSVFile

function CSVFile.new(class)
  local self = {}
  
  setmetatable(self,{
    __index = class,
  })
  return self
end 

function CSVFile:read(filePath)
  local handle = io.open(filePath, "rb")
  if not handle then return end
  
  for line in FileLines(handle) do
    if (not self.descr) then
      self.descr = {}
      self.data = {}
      self.lookup = {}
      local n = 1
      for word in string.gmatch(line,'(%b""),*') do
        word = string.sub(word,2,-2)
        table.insert(self.descr, word)
        table.insert(self.data, {})
        self.lookup[word] = n
        n = n + 1
      end
    else
      local n = 1
      for word in string.gmatch(line,'(%b""),*') do
        word = string.sub(word,2,-2)
        word = string.gsub(word,",",".")
        table.insert(self.data[n], tonumber(word) or word)
        n = n + 1
      end
    end
    end
  handle:close()
  
  return self
end

function CSVFile:rename(from,to)
  local idx = self.lookup[from]
  self.lookup[to] = idx
  self.descr[idx] = to
  
  self.lookup[from] = nil
end

function CSVFile:getDescriptor()
  return self.descr
end

function CSVFile:merge(a,b)
  local output = self
  local inputs = b and {a,b} or {a}
  local checka = b and a or self
  local checkb = b or a
  
  if (b) then
    self.descr = {}
    self.lookup = {}
    self.data = {}
    
    local n = 1
    for i,v in ipairs(checka.descr) do
      if checka.lookup[v] then 
        table.insert(self.descr,v)
        self.lookup[v] = n 
        n = n + 1
      end
    end
    
    if (#self.descr < 1) then return nil end

    for i,v in ipairs(self.descr) do
      table.insert(self.data,{})
    end
  else
    for i,v in ipairs(checka.descr) do
      if checkb.descr[i] ~= v then 
        print("MisMatch:",checkb.descr[i],v)
        return nil 
      end
    end
  end
  
  for n,from in ipairs(inputs) do
    for i,v in ipairs(self.descr) do
      local merge = from.data[from.lookup[v]]
      local to = self.data[i]
      for n,elem in ipairs(merge) do
        table.insert(to,elem)
      end
    end
  end

  return self
end

function CSVFile:getData(what)
  return self.data[self.lookup[what]]
end

function CSVFile:setData(what,data)
  self.data[self.lookup[what]] = data
end

function CSVFile:appendData(what,data)
  self.descr = self.descr or {}
  self.data = self.data or {}
  self.lookup = self.lookup or {}
  table.insert(self.descr,what)
  self.lookup[what] = #self.descr
  self.data[#self.descr] = data
end

function CSVFile:sort(what,comp)
  assert(self.data)
  local ordered = {}
  local data = self.data[self.lookup[what]]
  local count = #data
  for i=1,count do 
    local key = {i,data[i]}
    table.insert(ordered,key)
  end
  
  table.sort(ordered,function(a,b) 
      return comp(a[2],b[2])
    end)
  
  for n,v in ipairs(self.data) do
    local newdata = {}
    for i=1,count do
      local key = ordered[i][1]
      table.insert(newdata, v[key])
    end
    self.data[n] = newdata
  end
end

function CSVFile:write(filePath, elems)
  assert(self.data)
  local handle = io.open(filePath, "wb")
  if not handle then return end
  
  local cnt = #self.descr
  for i,v in ipairs(self.descr) do
    handle:write('"'..v..'"'..((i < cnt) and "," or ""))
  end
  handle:write("\n")
  
  elems = elems or #self.data[1]
  for n=1,elems do
    for i=1,cnt do 
      local data = self.data[i][n]
      handle:write('"'..tostring(data)..'"'..((i < cnt) and "," or ""))
    end
    handle:write("\n")
  end
  handle:close()
end

function csv.CSVReadWideTable(filePath)
  local handle = io.open(filePath, "rb")
  if not handle then return end
  
  local descr
  local out = {}
  for line in FileLines(handle) do
    if (not descr) then
      descr = {}
      local n = 1
      for word in string.gmatch(line,'(%b""),*') do
        word = string.sub(word,2,-2)
        table.insert(descr, word)
        n = n + 1
      end
    else
      local n = 1
      local item = {}
      for word in string.gmatch(line,'(%b""),*') do
        word = string.sub(word,2,-2)
        word = string.gsub(word,",",".")
        item[descr[n]] = tonumber(word) or word
        n = n + 1
      end
      table.insert(out,item)
    end
  end
  handle:close()
  
  return out
end

function csv.CSVIterateWideTable(filePath,it)
  local handle = io.open(filePath, "rb")
  if not handle then return end
  
  local descr
  for line in FileLines(handle) do
    if (not descr) then
      descr = {}
      local n = 1
      for word in string.gmatch(line,'(%b""),*') do
        word = string.sub(word,2,-2)
        table.insert(descr, word)
        n = n + 1
      end
    else
      local n = 1
      local item = {}
      for word in string.gmatch(line,'(%b""),*') do
        word = string.sub(word,2,-2)
        word = string.gsub(word,",",".")
        item[descr[n]] = tonumber(word) or word
        n = n + 1
      end
      it(item)
    end
  end
  handle:close()
end

function csv.CSVWriteWideTable(tab,filePath, descr, dontwritedescr, header)
  if (not tab[1]) then 
    print("table has no content")
    return 
  end
  local handle = io.open(filePath, "wt")
  if not handle then 
    print("cannot open file:", filePath)
    return 
  end
  
  if (header) then
    handle:write(header)
    handle:write("\n")
  end
  
  if (not descr) then
    descr = {}
    for i,v in pairs(tab[1]) do
      table.insert(descr,i)
    end
    table.sort(descr)
  else
    local lk = {}
    local ndescr = {}
    for i,v in ipairs(descr) do
      lk[v] = true
      ndescr[i] = v
    end
    
    local ndescrrest = {}
    for i,v in pairs(tab[1]) do
      if (not lk[i]) then
        table.insert(ndescrrest,i)
      end
    end
    table.sort(ndescrrest)
    for i,v in ipairs(ndescrrest) do
      table.insert(ndescr,v)
    end
    descr = ndescr
  end
  
  local cnt = #descr
  if (not dontwritedescr) then
    for i,v in ipairs(descr) do
      handle:write('"'..v..'"'..((i < cnt) and ", " or ""))
    end
    handle:write("\n")
  end
  
  for i,v in ipairs(tab) do
    for n=1,cnt do
      local data = v[descr[n]]
      local notnumber = tonumber(data) == nil
      local wrap = notnumber and '"' or ""
      handle:write(wrap..(data and tostring(data) or "")..wrap..((n < cnt) and ", " or ""))
    end
    handle:write("\n")
  end
  
  handle:close()
end

return csv

