
local function init(settings,lprint)
	settings = settings or {port = "55555"}
	local zmq = require "zmq"
	local ctx = zmq.init(1)
	local s = ctx:socket(zmq.REP)
	local addr = "tcp://127.0.0.1:"..settings.port
	
	print("server:",addr)
	if (not s:bind(addr)) then 
		print("server: failed")
		return 
	end

	local function poll()
		local t,zmerr = s:recv(zmq.NOBLOCK)
		if (zmerr == "timeout") then return end
		s:send("OK")
		if (t) then
			local fn,err = loadstring(t)
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



