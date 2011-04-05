if not (RELPATH) then
  RELPATH = function(str) return str end

	-- http://lua-users.org/wiki/EnhancedFileLines
	function FileLines(f)
		local CHUNK_SIZE = 1024
		local buffer = ""
		local pos_beg = 1
		return function()
			local pos, chars
			while 1 do
				pos, chars = buffer:match('()([\r\n].)', pos_beg)
				if pos or not f then
					break
				elseif f then
					local chunk = f:read(CHUNK_SIZE)
					if chunk then
						buffer = buffer:sub(pos_beg) .. chunk
						pos_beg = 1
					else
						f = nil
					end
				end
			end
			if not pos then
				pos = #buffer
			elseif chars == '\r\n' then
				pos = pos + 1
			end
			local line = buffer:sub(pos_beg, pos)
			pos_beg = pos + 1
			if #line > 0 then
				return line
			end    
		end
	end
	
	function StripCommentsC(tx)
		local out = ""
		local lastc = ""
		local skip
		local skipline
		local skipmulti
		local tx = string.gsub(tx, "\r\n", "\n")
		for c in tx:gmatch(".") do
			local oc = c
			local tu = lastc..c
			skip = c == '/'
			
			if ( not (skipmulti or skipline)) then
				if (tu == "//") then
					skipline = true
				elseif (tu == "/*") then
					skipmulti = true
					c = ""
				elseif (lastc == '/') then
					oc = tu
				end
			elseif (skipmulti and tu == "*/") then
				skipmulti = false
				c = ""
			elseif (skipline and lastc == "\n") then
				out = out.."\n"
				skipline = false
			end
			
			lastc = c
			if (not (skip or skipline or skipmulti)) then
				out = out..oc
			end
		end
		
		return out..lastc
	end

end
