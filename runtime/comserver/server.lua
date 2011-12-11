
local function init(settings)
  local settings = settings or {port = "55555"}
  local zmq = require "zmq"
  local ctx = zmq.init(1)
  local s = ctx:socket(zmq.SUB)
  local addr = "tcp://127.0.0.1:"..settings.port
  
  print("server:",addr)
  if (not s:bind(addr)) then 
    print("server: failed")
    return 
  end
  
  s:setopt(zmq.SUBSCRIBE,"LX2")

  local function poll()
    io.flush()
    local t,zmerr = s:recv(zmq.NOBLOCK)
    if (zmerr == "timeout") then return end
    if (t) then
      local fn,err = loadstring(string.sub(t,4,-1))
      if not fn then
        print("Error: "..err)
      else
        xpcall(fn,function(err)
          print(debug.traceback(err))
        end)
      end
    end
  end

  return poll
end

return init



