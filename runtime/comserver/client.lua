
local function init(settings)
	local settings = settings or {port = "55555"}
	local zmq = require "zmq"
	local ctx = zmq.init(1)
	local s = ctx:socket(zmq.PUB)
	local addr = "tcp://127.0.0.1:"..settings.port
	
	print("client:",addr)
	if (not s:connect(addr)) then 
		print("client: failed")
		return 
	end

	local function send(msg)
		s:send("LX2"..msg,zmq.NOBLOCK)
	end
	
	return send
end

return init
